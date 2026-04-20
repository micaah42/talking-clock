import QtQuick
import QtQuick.Controls
import Clock

import "../Style"

DelayButton {
    id: root
    property bool wasActivated: false
    onActivated: wasActivated = true
    onReleased: progress = 0

    background: Rectangle {
        anchors.fill: parent

        border.width: 3.5
        SequentialAnimation on border.color {
            running: !root.pressed && !root.wasActivated
            loops: Animation.Infinite

            ColorAnimation {
                from: 'white'
                to: Theme.accent
                easing.type: Easing.InOutQuad
                duration: 1000
            }
            ColorAnimation {
                from: Theme.accent
                to: 'white'
                easing.type: Easing.InOutQuad
                duration: 1000
            }
        }

        radius: 12
        gradient: Gradient {
            orientation: Qt.Horizontal

            GradientStop {
                color: Theme.accent
            }
            GradientStop {
                color: Theme.accentColor(Material.Shade300)

                SequentialAnimation on position {
                    loops: Animation.Infinite
                    running: !root.wasActivated

                    PropertyAnimation {
                        easing.type: Easing.InOutQuad
                        duration: 1000
                        from: 1e-6
                        to: 1 - 1e-6
                    }

                    PropertyAnimation {
                        easing.type: Easing.InOutQuad
                        duration: 1000
                        from: 1 - 1e-6
                        to: 1e-6
                    }
                }
            }
            GradientStop {
                color: Theme.accent
                position: 1
            }
        }
    }

    contentItem: Item {
        anchors.fill: parent

        Rectangle {
            id: wiggly

            anchors.fill: parent
            anchors.margins: 16 * (1 - root.progress) + 2

            border.color: Theme.primaryColor(Material.Shade300)
            border.width: 3.5

            color: root.pressed ? Qt.darker(Theme.primary, 1.2) : Theme.primary
            radius: 12

            CLabel {
                anchors.centerIn: parent
                font.family: FontService.family
                font.pixelSize: 32
                text: root.text
            }
        }
    }
}
