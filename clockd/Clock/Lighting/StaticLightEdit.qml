import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import Clock
import QLighting

import "../Style"
import "../Controls"

ColumnLayout {
    id: root

    property StaticLight lightMode

    spacing: 16

    CLabel {
        text: "Static Light Settings"
        size: CLabel.Large
        Layout.fillWidth: true
    }

    ColumnLayout {
        Layout.fillWidth: true
        spacing: 12

        CLabel {
            text: "Color"
            size: CLabel.Medium
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            ColorPicker {
                Layout.preferredWidth: 80
                Layout.preferredHeight: 80
                value: lightMode.color

                onCurrentColorChanged: {
                    if (value !== lightMode.color) {
                        lightMode.color = color
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 8

                CTextField {
                    Layout.fillWidth: true
                    placeholderText: "Hex color"
                    text: lightMode.color.toString()
                    onEditingFinished: {
                        try {
                            lightMode.color = text
                        } catch (e) {

                            // Invalid color, revert
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }
    }
}
