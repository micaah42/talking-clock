import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import Clock
import QLighting

import "../Style"
import "../Controls"

ColumnLayout {
    id: root

    property WavingLight lightMode

    spacing: 16

    CLabel {
        text: "Waving Light Settings"
        size: CLabel.Large
        Layout.fillWidth: true
    }

    ColumnLayout {
        Layout.fillWidth: true
        spacing: 12

        // Length Slider
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            RowLayout {
                CLabel {
                    text: "Wavelength"
                    size: CLabel.Medium
                }

                Item {
                    Layout.fillWidth: true
                }

                CLabel {
                    text: lightMode.length.toString()
                    size: CLabel.Small
                }
            }

            LargeSlider {
                Layout.fillWidth: true
                from: 10
                to: 200
                stepSize: 1
                value: lightMode.length

                onValueChanged: {
                    if (Math.abs(value - lightMode.length) > 0.5) {
                        lightMode.length = Math.round(value)
                    }
                }
            }
        }

        // Color A
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            CLabel {
                text: "Color A"
                size: CLabel.Medium
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                ColorPicker {
                    Layout.preferredWidth: 60
                    Layout.preferredHeight: 60
                    value: lightMode.a
                    onCurrentColorChanged: {
                        if (value !== lightMode.a) {
                            lightMode.a = color
                        }
                    }
                }

                CTextField {
                    Layout.fillWidth: true
                    placeholderText: "Hex color"
                    text: lightMode.a.toString()
                    onEditingFinished: {
                        try {
                            lightMode.a = text
                        } catch (e) {

                            // Invalid color, revert
                        }
                    }
                }
            }
        }

        // Color B
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            CLabel {
                text: "Color B"
                size: CLabel.Medium
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                ColorPicker {
                    Layout.preferredWidth: 60
                    Layout.preferredHeight: 60
                    value: lightMode.b
                    onCurrentColorChanged: {
                        if (value !== lightMode.b) {
                            lightMode.b = color
                        }
                    }
                }

                CTextField {
                    Layout.fillWidth: true
                    placeholderText: "Hex color"
                    text: lightMode.b.toString()
                    onEditingFinished: {
                        try {
                            lightMode.b = text
                        } catch (e) {

                            // Invalid color, revert
                        }
                    }
                }
            }
        }
    }
}
