import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import Clock
import QLighting

import "../Style"
import "../Controls"

ColumnLayout {
    id: root

    property PerlinLight2D lightMode

    spacing: 16

    CLabel {
        text: "Perlin Light 2D Settings"
        size: CLabel.Large
        Layout.fillWidth: true
    }

    ColumnLayout {
        Layout.fillWidth: true
        spacing: 12

        // Stretch X Slider
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            RowLayout {
                CLabel {
                    text: "Stretch X"
                    size: CLabel.Medium
                }

                Item {
                    Layout.fillWidth: true
                }

                CLabel {
                    text: lightMode.stretchX.toFixed(3)
                    size: CLabel.Small
                }
            }

            LargeSlider {
                Layout.fillWidth: true
                from: 0.001
                to: 0.1
                stepSize: 0.001
                value: lightMode.stretchX

                onValueChanged: {
                    if (Math.abs(value - lightMode.stretchX) > 0.0005) {
                        lightMode.stretchX = value
                    }
                }
            }
        }

        // Stretch T (Time) Slider
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            RowLayout {
                CLabel {
                    text: "Stretch T (Time)"
                    size: CLabel.Medium
                }

                Item {
                    Layout.fillWidth: true
                }

                CLabel {
                    text: lightMode.stretchT.toFixed(3)
                    size: CLabel.Small
                }
            }

            LargeSlider {
                Layout.fillWidth: true
                from: 0.001
                to: 0.1
                stepSize: 0.001
                value: lightMode.stretchT

                onValueChanged: {
                    if (Math.abs(value - lightMode.stretchT) > 0.0005) {
                        lightMode.stretchT = value
                    }
                }
            }
        }

        // Frequency Slider
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            RowLayout {
                CLabel {
                    text: "Frequency"
                    size: CLabel.Medium
                }

                Item {
                    Layout.fillWidth: true
                }

                CLabel {
                    text: lightMode.frequency.toFixed(2)
                    size: CLabel.Small
                }
            }

            LargeSlider {
                Layout.fillWidth: true
                from: 0.5
                to: 8.0
                stepSize: 0.1
                value: lightMode.frequency

                onValueChanged: {
                    if (Math.abs(value - lightMode.frequency) > 0.05) {
                        lightMode.frequency = value
                    }
                }
            }
        }

        // Persistence Slider
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            RowLayout {
                CLabel {
                    text: "Persistence"
                    size: CLabel.Medium
                }

                Item {
                    Layout.fillWidth: true
                }

                CLabel {
                    text: lightMode.persistence.toFixed(2)
                    size: CLabel.Small
                }
            }

            LargeSlider {
                Layout.fillWidth: true
                from: 0.0
                to: 1.0
                stepSize: 0.05
                value: lightMode.persistence

                onValueChanged: {
                    if (Math.abs(value - lightMode.persistence) > 0.025) {
                        lightMode.persistence = value
                    }
                }
            }
        }

        // Octaves Spinner
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            CLabel {
                text: "Octaves"
                size: CLabel.Medium
            }

            CSpinBox {
                Layout.fillWidth: true
                spinBox.from: 1
                spinBox.to: 8
                spinBox.value: lightMode.octaves
                spinBox.onValueChanged: {
                    if (value !== lightMode.octaves) {
                        lightMode.octaves = value
                    }
                }
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
