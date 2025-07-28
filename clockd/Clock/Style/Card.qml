import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import Clock 1.0

Item {
    property bool bright: false
    property real backgroundOpacity: 1

    Rectangle {
        anchors.fill: parent

        // color: bright ? Theme.primary : Theme.darkPrimary
        color: bright ? Theme.primaryColor(Material.ShadeA700) : //@
                        Qt.darker(Theme.primaryColor(Material.Shade900), 1.42)

        // opacity: backgroundOpacity
        border.color: Qt.darker(color, 1.25)
        border.width: 0.75
        radius: 8
    }

    MouseArea {
        anchors.fill: parent
    }
}
