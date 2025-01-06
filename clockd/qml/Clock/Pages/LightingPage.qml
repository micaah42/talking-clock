import QtQuick 2.14
import QtQuick.Layouts
import QtQuick.Controls.Material

import Clock
import Clock.Style
import Clock.Controls

ColumnLayout {
    property StaticLight staticLight: Lighting.staticLight
    property WavingLight wavingLight: Lighting.wavingLight
    property PulsatingLight pulsatingLight: Lighting.pulsatingLight

    readonly property var availableLightModes: Array.from(swipeView.contentChildren).map(x => x.lightMode)
    spacing: 0

    RowLayout {
        Layout.minimumHeight: 56
        Layout.maximumHeight: 56
        Layout.bottomMargin: 16
        spacing: 24

        Button {
            Layout.preferredWidth: 2 * height
            Layout.fillHeight: true

            onClicked: Lighting.enabled = !Lighting.enabled
            highlighted: Lighting.enabled
            bottomInset: 0
            topInset: 0

            Icon {
                anchors.centerIn: parent
                text: Icons.power_settings_new
            }
        }

        LargeSlider {
            Layout.fillHeight: true
            Layout.fillWidth: true
            labelText: 'Brightness'

            value: Lighting.brightness
            onValueChanged: {
                Lighting.brightness = value
                Lighting.enabled = true
            }
        }
    }

    RowLayout {
        enabled: Lighting.enabled

        Repeater {
            model: availableLightModes
            delegate: Button {
                Layout.fillWidth: true
                highlighted: modelData === Lighting.mode
                onClicked: Lighting.mode = modelData
                text: modelData.name
                implicitWidth: 0
            }
        }
    }

    Item {
        Layout.fillHeight: true
        Layout.fillWidth: true

        LightingDisplay {
            lighting: Lighting
            width: parent.width
            height: 8
        }

        Frame {
            anchors.fill: parent

            SwipeView {
                id: swipeView
                anchors.fill: parent
                anchors.margins: 8
                currentIndex: availableLightModes.indexOf(Lighting.mode)
                enabled: Lighting.enabled
                interactive: false
                spacing: 32
                clip: true

                RowLayout {
                    property LightMode lightMode: staticLight
                    spacing: 32

                    ColorPicker {
                        onCurrentColorChanged: staticLight.color = currentColor
                        value: staticLight.color
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }

                    GridLayout {
                        id: palettes

                        property var currentPalette: Palettes.palettes[currentIndex % Palettes.palettes.length]
                        property int currentIndex: 0

                        Layout.maximumWidth: parent.width / 3
                        Layout.fillHeight: true

                        columns: 3
                        rows: 4

                        CToolButton {
                            onClicked: palettes.currentIndex -= 1
                            text: Icons.chevron_backward
                        }
                        CLabel {
                            horizontalAlignment: Text.AlignHCenter
                            Layout.fillWidth: true
                            text: palettes.currentPalette.name
                        }

                        CToolButton {
                            onClicked: palettes.currentIndex += 1
                            text: Icons.chevron_forward
                        }

                        Repeater {
                            model: palettes.currentPalette.colors
                            delegate: Button {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                onClicked: staticLight.color = modelData
                                Material.background: modelData
                                bottomInset: 0
                                topInset: 0
                            }
                        }
                    }
                }

                RowLayout {
                    property LightMode lightMode: wavingLight
                    spacing: 16

                    ColorPicker {
                        onCurrentColorChanged: wavingLight.a = currentColor
                        value: wavingLight.a
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }
                    ColorPicker {
                        onCurrentColorChanged: wavingLight.b = currentColor
                        value: wavingLight.b
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }
                    ColumnLayout {
                        CSpinBox {
                            labelText: 'Length'
                            spinBox {
                                onValueChanged: wavingLight.length = spinBox.value
                                value: wavingLight.length
                                stepSize: 5
                                from: 5
                                to: 250
                            }
                        }
                        CSpinBox {
                            labelText: 'Speed'
                            spinBox.onValueChanged: wavingLight.speed = spinBox.value
                            spinBox.value: wavingLight.speed
                        }
                        Item {
                            Layout.fillHeight: true
                        }
                    }
                }

                RowLayout {
                    Component.onCompleted: console.log(pulsatingLight, this, width, height)
                    property LightMode lightMode: pulsatingLight
                    spacing: 16

                    ColorPicker {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        onCurrentColorChanged: pulsatingLight.a = currentColor
                        value: pulsatingLight.a
                    }
                    ColorPicker {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        onCurrentColorChanged: pulsatingLight.b = currentColor
                        value: pulsatingLight.b
                    }
                    ColumnLayout {
                        Layout.preferredWidth: 220
                        Slider {
                            onValueChanged: pulsatingLight.f1 = value
                            value: pulsatingLight.f1
                            from: 1
                            to: 10
                        }
                        Slider {
                            onValueChanged: pulsatingLight.f2 = value
                            value: pulsatingLight.f2
                            from: 0.01
                            to: 1
                        }
                        Slider {
                            onValueChanged: pulsatingLight.f3 = value
                            value: pulsatingLight.f3
                            from: 0.01
                            to: 1
                        }
                        Item {
                            Layout.fillHeight: true
                        }
                    }
                }
            }
        }
    }
}
