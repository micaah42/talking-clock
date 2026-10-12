"""Shared fixtures for clockd integration tests."""

import os
import signal
import subprocess
import time
from pathlib import Path

import pytest
from websocket import WebSocketException

from qopenremote_ws import QOpenRemoteWebSocket
from quicktestengine import QuickTestEngine
from windowrecorder import create_ffmpeg_subprocess, find_window_id


def process_exit_status(process: subprocess.Popen[str]) -> str:
    """Format a process's completed exit status for test diagnostics."""
    if process.returncode is None:
        return "is still running"
    if process.returncode < 0:
        signal_number = -process.returncode
        try:
            return f"was terminated by signal {signal.Signals(signal_number).name}"
        except ValueError:
            return f"was terminated by signal {signal_number}"
    return f"exited with code {process.returncode}"


@pytest.fixture
def clockd_process(request: pytest.FixtureRequest) -> subprocess.Popen[str]:
    """Run clockd and record its window to a test-named video beside the target."""
    target_path = os.environ.get("CLOCKD_TEST_TARGET")
    if target_path is None:
        pytest.fail("CLOCKD_TEST_TARGET must name the clockd executable")

    database_path = Path(__file__).parents[1] / "share" / "clockd" / "actiondays.sqlite"
    environment = os.environ.copy()
    environment.update(
        {
            "DB_DATABASE": str(database_path),
            # "QT_QPA_PLATFORM": "offscreen",
        }
    )
    clockd_process = subprocess.Popen(
        [target_path],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        env=environment,
    )
    recording_path = (
        Path(target_path).parent
        / "current-test-recordings"
        / f"{request.node.name}.mp4"
    )
    recording_path.parent.mkdir(parents=True, exist_ok=True)

    deadline = time.monotonic() + 3
    while True:
        try:
            recording_process = create_ffmpeg_subprocess(
                find_window_id("talking-clock"), recording_path
            )
            break
        except LookupError:
            if clockd_process.poll() is not None:
                output, _ = clockd_process.communicate()
                pytest.fail(
                    "clockd "
                    f"{process_exit_status(clockd_process)} before its window appeared:\n"
                    f"{output}"
                )
            if time.monotonic() >= deadline:
                pytest.fail("clockd window did not appear within deadline")
            time.sleep(0.05)

    try:
        yield clockd_process
    finally:
        try:
            if recording_process.stdin is not None:
                recording_process.stdin.write("q\n")
                recording_process.stdin.flush()
            recording_process.wait(timeout=3)
        except (BrokenPipeError, subprocess.TimeoutExpired):
            recording_process.kill()
            recording_process.wait()
        clockd_process.terminate()
        try:
            output, _ = clockd_process.communicate(timeout=3)
        except subprocess.TimeoutExpired:
            clockd_process.kill()
            output, _ = clockd_process.communicate()
        print(f"\n--- clockd exit status: {process_exit_status(clockd_process)} ---")
        if output:
            print(f"\n--- clockd logs ---\n{output}")


@pytest.fixture
def quick_test_engine(clockd_process: subprocess.Popen[str]) -> QuickTestEngine:
    """Connect to the QuickTestEngine instance created by clockd's main function."""
    time.sleep(0.5)

    if clockd_process.poll() is not None:
        output, _ = clockd_process.communicate()
        pytest.fail(
            f"clockd {process_exit_status(clockd_process)} during startup:\n{output}"
        )

    client = QOpenRemoteWebSocket(timeout=15)
    deadline = time.monotonic() + 3
    try:
        while True:
            try:
                client.connect()
                break
            except (OSError, WebSocketException):
                if clockd_process.poll() is not None:
                    output, _ = clockd_process.communicate()
                    pytest.fail(
                        "clockd "
                        f"{process_exit_status(clockd_process)} before accepting connections:\n"
                        f"{output}"
                    )
                if time.monotonic() >= deadline:
                    pytest.fail(
                        "clockd did not accept WebSocket connections within deadline"
                    )
                time.sleep(0.05)

        yield QuickTestEngine(client)
    finally:
        client.disconnect()
