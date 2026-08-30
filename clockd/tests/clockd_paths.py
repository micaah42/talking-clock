from quicktestengine import QuickTestEngine, Path, PathPart, RecordingFrame, Recording

MAINSCREEN_PATH: Path = [
    PathPart(id="window"),
    PathPart(id="loader"),
    PathPart(objectName="clockMain"),
]

MAINSCREEN_MOUSEAREA_PATH: Path = [
    *MAINSCREEN_PATH,
    PathPart(typeName="QQuickMouseArea"),
]

MAINSCREEN_SIDEBAR_TABBAR_PATH: Path = [
    *MAINSCREEN_PATH,
    PathPart(id="menuDrawer"),
    PathPart(id="sidebarContainer"),
    PathPart(id="sidebar"),
    PathPart(id="sideBarItem"),
    PathPart(id="tabBar"),
]
