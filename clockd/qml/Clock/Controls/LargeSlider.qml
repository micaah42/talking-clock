import QtQuick 2.14
import QtQuick.Controls
import QtQuick.Controls.Material

import Clock
import Clock.Controls

Slider {
    id: root

    property alias labelText: label.text
    property alias label: label
    property real radius: 12

    snapMode: Slider.SnapAlways
    implicitHeight: 40
    stepSize: 0.025

    background: Rectangle {
        id: background
        anchors.fill: parent
        border.color: Material.foreground
        border.width: root.pressed ? 2.5 : 2

        color: ColorService.primary
        opacity: enabled ? 1 : 0.72
        radius: root.radius

        Item {
            anchors.fill: parent
            anchors.margins: parent.border.width

            Rectangle {
                color: root.pressed ? Qt.lighter(ColorService.accent, 1.25) : ColorService.accent
                opacity: enabled ? 1 : 0.72

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
