"""Integration tests for the clockd application."""

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
    assert quick_test_engine.click(MAINSCREEN_MOUSEAREA_PATH)

    count = quick_test_engine.findAwait(
        [PathPart(propertyName="count")] + MAINSCREEN_SIDEBAR_TABBAR_PATH
    )
    for i in range(count):
        quick_test_engine.click([PathPart(index=i)] + MAINSCREEN_SIDEBAR_TABBAR_PATH)
