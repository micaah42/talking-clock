import QtQuick 
import QtQuick.Controls
import QtQuick.Controls.Material

import Clock
import "../Style"
import "../Controls"

Slider {
    id: root

    property alias labelText: label.text
    property alias label: label
    property real radius: 12

    snapMode: Slider.SnapAlways
    implicitHeight: 40
    stepSize: 0.005

    background: Rectangle {
        id: background
        anchors.fill: parent
        border.color: Material.foreground
        border.width: root.pressed ? 2 : 1.5

        color: Theme.primary
        opacity: enabled ? 1 : 0.72
        radius: root.radius

        Item {
            anchors.fill: parent
            anchors.margins: parent.border.width

            Rectangle {
                color: root.pressed ? Theme.accentColor(Material.Shade300) : Theme.accent
                opacity: enabled ? 1 : Theme.o72

                width: height + root.visualPosition * (parent.width - height)
                radius: root.radius - background.border.width
                height: parent.height

                Behavior on color {
                    ColorAnimation {
                        duration: 100
                    }
                }
            }
        }
    }

    handle: Item {}

    CLabel {
        id: label
        anchors.centerIn: parent
        font.pixelSize: 24
        topPadding: 4
    }
}
