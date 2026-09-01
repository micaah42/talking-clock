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
def clockd() -> subprocess.Popen[str]:
    """Run clockd with its bundled database using Qt's offscreen backend."""
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
    process = subprocess.Popen(
        [target_path],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        env=environment,
    )

    try:
        yield process
    finally:
        process.terminate()
        try:
            output, _ = process.communicate(timeout=3)
        except subprocess.TimeoutExpired:
            process.kill()
            output, _ = process.communicate()
        print(f"\n--- clockd exit status: {process_exit_status(process)} ---")
        if output:
            print(f"\n--- clockd logs ---\n{output}")


@pytest.fixture
def quick_test_engine(clockd: subprocess.Popen[str]) -> QuickTestEngine:
    """Connect to the QuickTestEngine instance created by clockd's main function."""
    time.sleep(0.5)
    
    if clockd.poll() is not None:
        output, _ = clockd.communicate()
        pytest.fail(f"clockd {process_exit_status(clockd)} during startup:\n{output}")

    client = QOpenRemoteWebSocket(timeout=15)
    deadline = time.monotonic() + 3
    try:
        while True:
            try:
                client.connect()
                break
            except (OSError, WebSocketException):
                if clockd.poll() is not None:
                    output, _ = clockd.communicate()
                    pytest.fail(
                        "clockd "
                        f"{process_exit_status(clockd)} before accepting connections:\n"
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
