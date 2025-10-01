import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.VirtualKeyboard
import QtMultimedia

import Clock
import NetworkManagerQml

import "../Alarms"
import "../Controls"
import "../Components"
import "../Lighting"
import "../Style"
import "."

Item {
    property real sidebarWidth: width / 2
    property real drawerHeight: 148

    Component.onCompleted: {
        EventFilter.installToObject(window)
        LightingInit.init()
    }

    SpaceScene {
        anchors.fill: parent
    }

    Clock {
        anchors.bottom: drawer.top
        anchors.left: sideBar.right
        anchors.right: parent.right
        anchors.top: parent.top

        scale: sideBar.open ? 0.69 : 1
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

    CFrame {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 16
        width: parent.width / 2 - 2 * anchors.margins

        opacity: NetworkManagerQml.connectivity === 4 ? connChangedTimer.running : 1

        Behavior on opacity {
            OpacityAnimator {}
        }

        contentItem: SimpleListItem {
            id: connectivity
            descriptionItem.wrapMode: Text.Wrap

            property int con: {
                switch (NetworkManagerQml.connectivity) {
                case 0:
                    icon = Icons.cloud_sync
                    title = 'Unknown Connectivity'
                    description = 'Network connectivity is unknown.'
                    break
                case 1:
                    icon = Icons.cloud_alert
                    title = 'No Connectivity'
                    description = 'The host is not connected to any network.'
                    break
                case 2:
                    icon = Icons.captive_portal
                    title = 'Portal'
                    description = 'The host is behind a captive portal and cannot reach the full Internet.'
                    break
                case 3:
                    icon = Icons.cloud_alert
                    title = 'Limited'
                    description = 'The host is connected to a network, but does not appear to be able to reach the full Internet.'
                    break
                case 4:
                    icon = Icons.cloud_done
                    title = 'Full'
                    description = 'The host is connected to a network, and appears to be able to reach the full Internet.'
                    break
                }

                connChangedTimer.start()
                return NetworkManagerQml.connectivity
            }

            Timer {
                id: connChangedTimer
                interval: 15000
            }
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
            anchors.bottomMargin: 4
            clip: true

            StackView {
                id: sideBarStack
                anchors.fill: parent
                anchors.margins: 16
                anchors.bottomMargin: 0
                initialItem: InfoItem {}
            }
        }
    }

    MainScreenNotifications {
        opacity: drawer.open || sideBar.open || mainMenu.currentPage !== null ? 0 : 1

        Behavior on opacity {
            PropertyAnimation {}
        }

        width: parent.width / 3
        x: 16
        y: 16
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

    FPSOverlay {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 16
        width: 300
        height: 80
    }
}
