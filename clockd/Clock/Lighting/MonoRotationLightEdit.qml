import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import Clock
import QLighting

import "../Style"
import "../Controls"

ColumnLayout {
    id: root

    property MonoRotationLight lightMode

    spacing: 16

    CLabel {
        text: "Mono Rotation Light Settings"
        size: CLabel.Large
        Layout.fillWidth: true
    }

    ColumnLayout {
        Layout.fillWidth: true
        spacing: 12

        // Waveform Selection
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            CLabel {
                text: "Wave Form"
                size: CLabel.Medium
            }

            CComboBox {
                Layout.fillWidth: true

                readonly property var values: [//@
                    MonoRotationLight.Sine, //@
                    MonoRotationLight.Sawtooth, //@
                    MonoRotationLight.Triangle, //@
                    MonoRotationLight.Square //@
                ]

                model: [//@
                    "Sine", //@
                    "Sawtooth", //@
                    "Triangle", //@
                    "Square" //@
                ]

                onActivated: i => lightMode.waveForm = values[i]
                currentIndex: values.indexOf(lightMode.waveForm)
            }
        }

        // Gradient Editor
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            CLabel {
                text: "Gradient"
                size: CLabel.Medium
            }

            CGradientEdit {
                Layout.fillWidth: true
                lightingGradient: lightMode.gradient
            }
        }
    }
}
