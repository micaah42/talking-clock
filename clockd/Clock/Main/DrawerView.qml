import QtQuick
import QtQuick.Layouts

import "../Style"

Item {
    id: root
    readonly property real sidebarWidth: width / 2
    readonly property real drawerHeight: 128

    property alias buttons: buttons
    property alias sidebar: sidebar
    property alias content: content
    property bool open: false

    Item {
        id: sidebarContainer
        height: root.height - drawerHeight
        width: sidebarWidth

        x: open ? 0 : -width
        Behavior on x {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
            }
        }

        Card {
            id: sidebar
            anchors.fill: parent
            anchors.margins: 16
            anchors.bottomMargin: -4
            clip: true
        }
    }

    Item {
        id: buttons
        height: drawerHeight
        width: root.width

        anchors.bottom: root.bottom
        anchors.bottomMargin: root.open ? 0 : -height

        Behavior on anchors.bottomMargin {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
            }
        }
    }

    Item {
        id: content
        anchors.right: root.right
        anchors.top: root.top
        anchors.left: sidebarContainer.right
        anchors.bottom: buttons.top

        scale: open ? 0.69 : 1
        transformOrigin: Item.Center
        Behavior on scale {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
            }
        }
    }
}
