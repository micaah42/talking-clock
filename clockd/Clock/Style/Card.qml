import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import Clock

Item {
    property bool bright: false
    property real backgroundOpacity: 1

    Rectangle {
        anchors.fill: parent

        // color: bright ? Theme.primary : Theme.darkPrimary
        color: bright ? Theme.primaryColor(Material.ShadeA700) : //@
                        Qt.darker(Theme.primaryColor(Material.Shade900), 1.56)

        // opacity: backgroundOpacity
        border.color: Qt.lighter(color, 1.25)
        border.width: 1.5
        radius: 8
    }

    MouseArea {
        anchors.fill: parent
    }
}
