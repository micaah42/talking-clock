import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14
import QtQuick.VirtualKeyboard
import QtMultimedia 5.15

import Clock 1.0
import Clock.Main 1.0
import Clock.Pages.AlarmPage
import Clock.Controls 1.0
import Clock.Style 1.0

Window {
    id: window

    onWidthChanged: console.warn(`window.width=${width}`)
    width: 1920 / 2

    onHeightChanged: console.warn(`window.height=${height}`)
    height: 1080 / 2

    visible: true
    color: ColorService.background

    Material.theme: Material.Dark
    Material.accent: ColorService.accent
    Material.primary: ColorService.primary
    Material.background: ColorService.background
    Material.roundedScale: Material.NotRounded

    property real drawerHeight: 164
    property real sidebarWidth: width / 3 + 2 * 16 / 3

    SpaceScene {
        anchors.fill: parent
    }

    Clock {
        anchors.bottom: drawer.top
        anchors.left: sideBar.right
        anchors.right: parent.right
        anchors.top: parent.top

        scale: sideBar.open ? 2 / 3 : 1
        transformOrigin: Item.Center
        Behavior on scale {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: drawer.open = !drawer.open
    }

    Item {
        id: drawer

        property bool open: false
        height: drawerHeight
        width: parent.width

        anchors.bottom: parent.bottom
        anchors.bottomMargin: open ? 0 : -height

        Behavior on anchors.bottomMargin {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
            }
        }

        MainMenu {
            id: mainMenu
            onCurrentPageChanged: drawer.open = false
            anchors.fill: parent
        }
    }

    Item {
        id: sideBar
        anchors.bottom: drawer.top
        anchors.top: parent.top
        width: sidebarWidth

        readonly property bool open: drawer.open || sideBarStack.depth > 1
        x: open ? 0 : -width
        Behavior on x {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
            }
        }

        Card {
            anchors.fill: parent
            anchors.margins: 16
            anchors.bottomMargin: 0
            clip: true

            StackView {
                id: sideBarStack
                anchors.fill: parent
                anchors.margins: 16

                initialItem: SwipeView {
                    spacing: 16

                    NextAlarm {
                        alarm: AlarmService.nextAlarm
                    }
                    ActionDayItem {}
                }
            }
        }
    }

    PageLoader {
        currentPage: mainMenu.currentPage
        onClosed: mainMenu.currentPage = null

        anchors.fill: parent
        anchors.margins: 8
        visible: scale !== 0
        scale: currentPage ? 1 : 0

        Behavior on scale {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
                duration: 250
            }
        }
    }

    Component {
        id: alarmNotification
        AlarmNotification {}
    }

    Connections {
        target: AlarmService
        function onAlarmTriggered(alarm) {
            const properties = {
                "alarm": alarm
            }

            sideBarStack.push(alarmNotification, properties)
            mainMenu.currentPage = null
            drawer.open = false
        }
    }

    Loader {
        id: keyboard
        anchors.fill: parent
        source: 'qrc:/Clock/Controls/Keyboard.qml'
        z: 1
    }

    Connections {
        target: EventFilter
        function onUserInactive() {
            mainMenu.currentPage = null
            drawer.open = false
        }
    }

    property var lastSwap: new Date()
    property real fps: 0

    onFrameSwapped: {
        const now = new Date()
        const diff = (now.getTime() - lastSwap.getTime())
        fps = 1000 / diff
        lastSwap = now
    }

    CLabel {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 8
        visible: SpaceTheme.fpsVisible
        font.pixelSize: 24
        text: fps.toFixed()
    }
}
