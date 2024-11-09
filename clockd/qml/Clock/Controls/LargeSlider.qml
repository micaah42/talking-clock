import QtQuick 2.14
import QtQuick.Controls
import QtQuick.Controls.Material

import Clock
import Clock.Controls

Slider {
    id: control

    property alias labelText: label.text
    property alias label: label

    snapMode: Slider.SnapAlways
    implicitHeight: 40
    stepSize: 0.025

    background: Rectangle {
        id: background
        anchors.fill: parent
        border.color: Material.foreground
        border.width: control.pressed ? 2.5 : 2

        color: ColorService.primary
        radius: height / 3
        opacity: enabled ? 1 : 0.72

        Item {
            anchors.fill: parent
            anchors.margins: parent.border.width

            Rectangle {
                color: control.pressed ? Qt.lighter(ColorService.accent, 1.25) : ColorService.accent
                opacity: enabled ? 1 : 0.72

                width: height + control.visualPosition * (parent.width - height)
                radius: height / 3 - background.border.width
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
