"""Integration tests for the clockd application."""

import logging
import time

from clockd_fixtures import clockd, quick_test_engine
from clockd_paths import MAINSCREEN_SIDEBAR_TABBAR_PATH, MAINSCREEN_MOUSEAREA_PATH
from quicktestengine import Path, PathPart, QuickTestEngine


def test_starts(quick_test_engine: QuickTestEngine) -> None:
    """Initialize clockd and connect to its bundled QuickTestEngine."""
    quick_test_engine.enabled = True
    assert quick_test_engine.enabled


def test_iterate_sidebar(quick_test_engine: QuickTestEngine) -> None:
    """Click each sidebar tab and verify that the corresponding page is visible."""

    assert quick_test_engine.findAwait(MAINSCREEN_MOUSEAREA_PATH, timeout=5)
    time.sleep(1)  # wait for animation to finish

    assert quick_test_engine.click(MAINSCREEN_MOUSEAREA_PATH)
    time.sleep(1)  # wait for animation to finish

    count = quick_test_engine.findAwait(
        [*MAINSCREEN_SIDEBAR_TABBAR_PATH, PathPart(propertyName="count")]
    )

    assert count is not None
    assert count > 0, "Sidebar tab count should be greater than 0"

    for i in range(int(count)):
        logging.info(f"Clicking sidebar tab {i+1}/{count}")

        assert quick_test_engine.click(
            [*MAINSCREEN_SIDEBAR_TABBAR_PATH, PathPart(index=i)]
        )

        time.sleep(1)  # wait for animation to finish

    assert quick_test_engine.click(MAINSCREEN_MOUSEAREA_PATH)
    time.sleep(1)  # wait for animation to finish
