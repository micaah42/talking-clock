import QtQuick 2.14
import QtQuick.Layouts
import QtQuick.Controls.Material

import Clock
import Clock.Style
import Clock.Controls

ColumnLayout {
    property StaticLight staticLight: Lighting.modes[0]
    property WavingLight wavingLight: Lighting.modes[1]
    property PrettyRandomLight prettyRandomLight: Lighting.modes[2]
    property PulsatingLight pulsatingLight: Lighting.modes[3]
    spacing: 0

    RowLayout {
        Layout.minimumHeight: 56
        Layout.maximumHeight: 56
        Layout.bottomMargin: 24
        spacing: 24

        Button {
            Layout.preferredWidth: height
            Layout.fillHeight: true
            implicitWidth: width
            highlighted: Lighting.enabled
            onClicked: Lighting.enabled = !Lighting.enabled

            checkable: true
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
            enabled: Lighting.enabled
            onValueChanged: Lighting.brightness = value
            value: Lighting.brightness
            labelText: 'Brightness'
        }
    }

    RowLayout {
        Layout.fillWidth: true
        Layout.bottomMargin: 16
        spacing: 0

        Repeater {
            id: repeater

            model: active ? Lighting.pixels : []
            delegate: Rectangle {
                id: d
                Layout.preferredHeight: 16
                Layout.fillWidth: true

                property Pixel pixel: modelData
                property color pixelColor: pixel.color
                color: Qt.rgba(pixelColor.r, pixelColor.g, pixelColor.b)

                Rectangle {
                    anchors.fill: parent
                    opacity: 0.75 * d.pixel.color.a
                    color: 'white'
                }
            }
        }
    }

    RowLayout {
        enabled: Lighting.enabled

        Repeater {
            model: Array.from(swipeView.contentChildren).map(x => x.lightMode)
            delegate: Button {
                Layout.fillWidth: true
                highlighted: modelData === Lighting.mode
                onClicked: Lighting.mode = modelData
                text: modelData.name
                implicitWidth: 0
            }
        }
    }

    Frame {
        Layout.fillHeight: true
        Layout.fillWidth: true

        SwipeView {
            id: swipeView
            anchors.fill: parent
            currentIndex: Lighting.modes.indexOf(Lighting.mode)
            enabled: Lighting.enabled
            interactive: false
            spacing: 32
            clip: true

            RowLayout {
                property LightMode lightMode: staticLight
                spacing: 16

                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    spacing: 16

                    ColorPicker {
                        onCurrentColorChanged: staticLight.color = currentColor
                        value: staticLight.color
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }

                    LargeSlider {
                        Layout.fillWidth: true
                        onValueChanged: staticLight.white = value
                        value: staticLight.white
                        labelText: 'W'
                    }
                }

                ColumnLayout {
                    id: palettes

                    property var currentPalette: Palettes.palettes[currentIndex % Palettes.palettes.length]
                    property int currentIndex: 0
                    Layout.maximumWidth: 220
                    Layout.fillHeight: true
                    spacing: 12

                    RowLayout {
                        Layout.fillWidth: true
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
                    }
                    GridLayout {
                        Layout.fillHeight: true

                        columns: 2
                        rows: 4

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
            }
            ColumnLayout {

                property LightMode lightMode: wavingLight
                CSpinBox {
                    labelText: 'Length'
                    spinBox.onValueChanged: wavingLight.length = spinBox.value
                    spinBox.value: wavingLight.length
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
            Item {
                property LightMode lightMode: prettyRandomLight
            }
            GridLayout {
                property LightMode lightMode: pulsatingLight
            }
        }
    }
}
