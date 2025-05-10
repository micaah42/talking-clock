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

        delegate: Frame {
            id: d

            property SystemLight light: modelData
            width: ListView.view.width

            contentItem: Expandable {
                id: contentItem
                title: `Light ${index + 1} - (${d.light.id.toUpperCase()})`
                label.Layout.preferredWidth: 120

                previewContent.children: Loader {
                    width: parent.width

                    Component {
                        id: sliderComponent
                        Item {
                            Slider {
                                anchors.centerIn: parent
                                width: parent.width
                                stepSize: d.light.stepSize ? 1 / d.light.stepSize : 1 / d.light.max
                                onValueChanged: d.light.brightness = value
                                value: d.light.brightness
                                from: d.light.min
                                to: 1
                            }
                        }
                    }
                    Component {
                        id: switchComponent
                        Item {
                            Switch {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                onClicked: d.light.brightness = checked ? 1 : 0
                                checked: d.light.brightness
                            }
                        }
                    }
                    sourceComponent: d.light.max === 1 ? switchComponent : sliderComponent
                }

                ValueDisplay {
                    size: ValueDisplay.Compact
                    valueText: d.light.path
                    labelText: 'Path'
                }
                ValueDisplay {
                    size: ValueDisplay.Compact
                    labelText: 'Brightness'
                    valueText: d.light.brightness
                }
                ValueDisplay {
                    size: ValueDisplay.Compact
                    labelText: 'Step Size'
                    valueText: d.light.stepSize
                }
                ValueDisplay {
                    size: ValueDisplay.Compact
                    labelText: 'Min'
                    valueText: d.light.min
                }
                ValueDisplay {
                    size: ValueDisplay.Compact
                    labelText: 'Max'
                    valueText: d.light.max
                }
            }
        }
    }
}
