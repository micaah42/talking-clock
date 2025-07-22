import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock
import Clock.Style
import Clock.Controls
import Clock.Components

Section {
    title: 'System Lights'
    icon: Icons.model_training

    ListView {
        Layout.fillHeight: true
        Layout.fillWidth: true
        spacing: 8
        clip: true

        model: SystemLightManager.lights

        delegate: CFrame {
            id: d

            property SystemLight light: modelData
            width: ListView.view.width

            contentItem: Expandable {
                id: contentItem

                previewContent: [
                    Switch {
                        onClicked: d.light.brightness = checked ? 1 : 0
                        checked: d.light.brightness
                    },
                    CLabel {
                        text: `Light ${index + 1} - (${d.light.id.toUpperCase()})`
                    },
                    Slider {
                        visible: d.light.max !== 1
                        Layout.fillWidth: true

                        stepSize: d.light.stepSize ? 1 / d.light.stepSize : 1 / d.light.max
                        onValueChanged: d.light.brightness = value
                        value: d.light.brightness
                        from: d.light.min
                        to: 1
                    },
                    Item {
                        visible: d.light.max === 1
                        Layout.fillWidth: true
                    }
                ]

                ValueDisplay {
                    value.wrapMode: Text.WrapAnywhere
                    size: ValueDisplay.Compact
                    valueText: d.light.path
                    labelText: 'Path'
                }
                RowLayout {
                    Item {
                        Layout.preferredHeight: l.height
                        Layout.fillWidth: true

                        ValueDisplay {
                            id: l
                            Layout.fillWidth: true
                            size: ValueDisplay.Compact
                            labelText: 'Brightness'
                            valueText: d.light.brightness
                        }
                    }
                    Item {
                        Layout.preferredHeight: l.height
                        Layout.fillWidth: true

                        ValueDisplay {
                            size: ValueDisplay.Compact
                            labelText: 'Step Size'
                            valueText: d.light.stepSize
                        }
                    }
                    Item {
                        Layout.preferredHeight: l.height
                        Layout.fillWidth: true

                        ValueDisplay {
                            size: ValueDisplay.Compact
                            labelText: 'Min'
                            valueText: d.light.min
                        }
                    }
                    Item {
                        Layout.preferredHeight: l.height
                        Layout.fillWidth: true

                        ValueDisplay {
                            size: ValueDisplay.Compact
                            labelText: 'Max'
                            valueText: d.light.max
                        }
                    }
                }
            }
        }
    }
}
