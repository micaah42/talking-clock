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
    property MonoRotationLight monoRotationLight: Lighting.monoRotationLight
    readonly property var availableLightModes: Array.from(swipeView.contentChildren).map(x => x.lightMode)

    readonly property var palettes: Palettes.palettes
    readonly property var currentPalette: Palettes.palettes[currentIndex % Palettes.palettes.length]
    property int currentIndex: 0

    spacing: 0

    RowLayout {
        Layout.minimumHeight: 56
        Layout.maximumHeight: 56
        spacing: 8

        Button {
            Layout.preferredWidth: 2 * height
            Layout.fillHeight: true

            Material.roundedScale: Material.MediumScale
            highlighted: Lighting.enabled
            onClicked: Lighting.enabled = !Lighting.enabled
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
                if (value === Lighting.brightness)
                    return
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
            height: 16
        }

        Frame {
            anchors.fill: parent
            topPadding: 24

            SwipeView {
                id: swipeView
                anchors.fill: parent
                anchors.margins: 8
                currentIndex: availableLightModes.indexOf(Lighting.mode)
                enabled: Lighting.enabled
                interactive: false
                spacing: 32
                clip: true

                GridLayout {
                    property LightMode lightMode: staticLight
                    columns: 8
                    rows: 6

                    Dialog {
                        id: customColorPopup
                        anchors.centerIn: Overlay.overlay
                        height: 7 * window.height / 8
                        width: 7 * window.width / 8

                        title: 'Custom Color'

                        contentItem: ColorPicker {
                            id: customColorPicker
                            onCurrentColorChanged: staticLight.color = currentColor
                            value: staticLight.color
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                        }
                    }

                    RowLayout {
                        Layout.columnSpan: parent.columns

                        CToolButton {
                            onClicked: currentIndex -= 1
                            text: Icons.chevron_backward
                        }
                        CLabel {
                            horizontalAlignment: Text.AlignHCenter
                            Layout.fillWidth: true
                            text: currentPalette.name
                        }

                        CToolButton {
                            onClicked: currentIndex += 1
                            text: Icons.chevron_forward
                        }
                    }

                    Repeater {
                        model: currentPalette.colors
                        delegate: Button {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            onClicked: staticLight.color = modelData
                            Material.background: modelData
                            implicitHeight: 0
                            bottomInset: 0
                            topInset: 0
                        }
                    }

                    Button {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        onClicked: customColorPopup.open()
                        Material.background: customColorPicker.currentColor
                        font.family: Icons.fontFamily
                        text: Icons.colors
                        implicitHeight: 0
                        bottomInset: 0
                        topInset: 0
                    }
                }
                GridLayout {
                    id: mono
                    onCurrentPaletteChanged: monoRotationLight.colors = currentPalette.colors
                    readonly property var currentPalette: Palettes.palettes[currentIndex % Palettes.palettes.length]
                    property int currentIndex: 0
                    property LightMode lightMode: monoRotationLight
                    columns: 8
                    rows: 6

                    RowLayout {
                        Layout.columnSpan: parent.columns

                        CToolButton {
                            onClicked: mono.currentIndex -= 1
                            text: Icons.chevron_backward
                        }
                        Item {
                            Layout.fillWidth: true
                            RowLayout {
                                anchors.centerIn: parent

                                CLabel {
                                    horizontalAlignment: Text.AlignHCenter
                                    text: mono.currentPalette.name
                                }
                                ToolButton {
                                    property int max: 5
                                    property int index: 0
                                    text: `${index + 1}x`

                                    onClicked: {
                                        index = (index + 1) % 5
                                        monoRotationLight.duration = 20000 / (index + 1)
                                    }
                                }
                            }
                        }
                        CToolButton {
                            onClicked: mono.currentIndex += 1
                            text: Icons.chevron_forward
                        }
                    }

                    Repeater {
                        model: mono.currentPalette.colors
                        delegate: Button {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            Material.background: modelData
                            implicitHeight: 0
                            bottomInset: 0
                            topInset: 0
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
