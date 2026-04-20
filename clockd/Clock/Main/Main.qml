import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.VirtualKeyboard
import QtMultimedia

import Clock
import NetworkManagerQml
import QPerformanceQml

import "../Alarms"
import "../Controls"
import "../Components"
import "../Lighting"
import "../Style"
import "."

Item {
    readonly property real sidebarWidth: width / 2
    readonly property real drawerHeight: 128

    Component.onCompleted: {
        EventFilter.installToObject(EventFilter.application)
        LightingInit.init()
        TimeService.now
    }

    Connections {
        target: EventFilter
        function onButtonClicked() {
            MaterialSounds.playUiTapVariant()
        }
    }

    SpaceScene {
        id: spaceScene
        anchors.fill: parent
    }

    MouseArea {
        anchors.fill: parent
        enabled: !sideBarItem.hasActiveAlarms
        onClicked: menuDrawer.open = !menuDrawer.open
        onReleased: clickCounter++
    }

    EdgeGlow {
        anchors.fill: parent
        opacity: 0

        SequentialAnimation on opacity {
            running: sideBarItem.hasActiveAlarms
            loops: Animation.Infinite
            alwaysRunToEnd: true

            OpacityAnimator {
                duration: 800
                from: 0
                to: 1
            }
            OpacityAnimator {
                duration: 800
                from: 1
                to: 0
            }
        }
    }

    DrawerView {
        id: menuDrawer
        anchors.fill: parent

        buttons.data: StackView {
            anchors.fill: parent
            anchors.margins: 16

            property Item selectedItem: sideBarItem.activeAlarms.length > 0 ? clearAlarms : mainMenu
            onSelectedItemChanged: replace(null, selectedItem)
            initialItem: Item {}

            property list<Item> items: [
                MainMenu {
                    id: mainMenu
                    onCurrentPageChanged: menuDrawer.open = false
                },
                RowLayout {
                    id: clearAlarms
                    spacing: 16

                    CDelayButton {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        text: 'Accept'
                        delay: 850

                        onClicked: {
                            sideBarItem.muteAlarms()
                        }
                        onReleased: {
                            if (!wasActivated)
                                return

                            sideBarItem.muteAlarms()
                            sideBarItem.activeAlarms.clear()

                            wasActivated = false
                        }
                    }
                    MainMenuButton {
                        Material.background: Theme.accentDark
                        Layout.preferredWidth: parent.width / 3
                        Layout.fillHeight: true
                        text: Icons.sleep
                    }
                }
            ]
        }
        sidebar.data: SideBarItem {
            id: sideBarItem
            anchors.fill: parent
            anchors.margins: 16

            onAlarmAdded: {
                mainMenu.currentPage = null
                menuDrawer.open = true
            }
        }
        content.data: Clock {
            anchors.fill: parent
        }
    }

    MainScreenNotifications {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 16

        opacity: mainMenu.currentPage !== null ? 0 : 1
        width: parent.width / 2 - 2 * anchors.margins

        Behavior on opacity {
            PropertyAnimation {}
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

    Keyboard {
        anchors.fill: parent
    }

    Connections {
        target: EventFilter
        function onUserInactive() {
            mainMenu.currentPage = null

            if (!sideBarItem.hasActiveAlarms)
                menuDrawer.open = false
        }
    }

    PerformancePopup {
        dialog.visible: SpaceTheme.fpsVisible
        Component.onCompleted: {
            Style.fontFamily = FontService.family
        }
    }

    property int clickCounter: 0
    onClickCounterChanged: {
        console.log(clickCounter)
        clickResetTimer.restart()

        if (clickCounter === 8)
            spaceScene.startRocket()
    }

    Timer {
        id: clickResetTimer
        onTriggered: clickCounter = 0
        interval: 1000
    }
}
