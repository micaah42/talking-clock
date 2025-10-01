import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import Clock
import QLighting

import "../Style"
import "../Controls"
import "../Lighting"

ColumnLayout {
    readonly property var modeScreens: [//@
        [staticLightComponent, StaticLight], //@
        [wavingLightComponent, WavingLight], //@
        [monoRotationLightComponent, MonoRotationLight], //@
        [perlinLightComponent, PerlinLight] //@
    ]

    RowLayout {
        Layout.minimumHeight: 56
        Layout.maximumHeight: 56
        spacing: 8

        Button {
            Layout.preferredWidth: 2 * height
            Layout.fillHeight: true

            Material.roundedScale: Material.MediumScale
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
                if (value === Lighting.brightness)
                    return
                Lighting.brightness = value
                Lighting.enabled = true
            }
        }
    }

    RowLayout {
        id: buttonsRow
        enabled: Lighting.enabled

        Repeater {
            model: LightingInit.modes
            delegate: Button {
                property LightMode lightMode: modelData
                Layout.fillWidth: true
                implicitWidth: 0

                onClicked: LightingInit.mode = lightMode
                checked: lightMode === LightingInit.mode
                highlighted: checked
                text: lightMode.name
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

        CFrame {
            anchors.fill: parent
            topPadding: 24

            StackView {
                id: stackView
                anchors.fill: parent
                anchors.margins: 8
                enabled: Lighting.enabled
                spacing: 32
                clip: true

                property Component comp: {
                    switch (LightingInit.mode.type) {
                    case LightMode.TypeStatic:
                        return staticLightComponent
                    case LightMode.TypeWaving:
                        return wavingLightComponent
                    case LightMode.TypeMonoRotation:
                        return monoRotationLightComponent
                    case LightMode.TypePerlin:
                        return perlinLightComponent
                    }
                }

                onCompChanged: {
                    clear()
                    push(comp)
                }

                initialItem: comp
            }
        }
    }

    Dialog {
        id: customColorPopup
        anchors.centerIn: Overlay.overlay
        height: 7 * window.height / 8
        width: 7 * window.width / 8

        title: 'Custom Color'
        contentItem: ColorPicker {
            id: customColorPicker
            // onCurrentColorChanged: valueEdited(currentColor)
            value: value
        }
    }

    Component {
        id: staticLightComponent
        Item {
            ColorPicker {
                anchors.fill: parent
                onCurrentColorChanged: StaticLight.color = currentColor
                value: StaticLight.color
            }
        }
    }
    Component {
        id: monoRotationLightComponent
        ListView {
            model: Palettes.palettes
            delegate: Item {
                height: frame.implicitHeight
                width: ListView.view.width

                CFrame {
                    id: frame
                    width: parent.width
                    ColumnLayout {
                        Label {
                            text: modelData.name
                        }
                        RowLayout {
                            Repeater {
                                model: modelData.colors
                                delegate: Button {
                                    Material.background: modelData
                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: MonoRotationLight.colors = modelData.colors
                }
            }
        }
    }
    Component {
        id: wavingLightComponent
        RowLayout {
            spacing: 16

            ColorPicker {
                onCurrentColorChanged: WavingLight.a = currentColor
                value: WavingLight.a
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            ColorPicker {
                onCurrentColorChanged: WavingLight.b = currentColor
                value: WavingLight.b
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            ColumnLayout {
                CSpinBox {
                    labelText: 'Length'
                    spinBox.onValueChanged: WavingLight.length = spinBox.value
                    spinBox.value: WavingLight.length
                    spinBox.stepSize: 5
                    spinBox.from: 5
                    spinBox.to: 250
                }
                CSpinBox {
                    labelText: 'Speed'
                    spinBox.onValueChanged: WavingLight.speed = spinBox.value / 10
                    spinBox.value: 10 * WavingLight.speed
                    spinBox.from: 1
                    spinBox.to: 25
                }
                Item {
                    Layout.fillHeight: true
                }
            }
        }
    }
    Component {
        id: perlinLightComponent
        ColumnLayout {
            Slider {
                Layout.fillWidth: true

                property var values: [0.1, 0.25, 0.5, 1, 1.5, 2]
                onValueChanged: PerlinLight.speed = values[value]
                value: Math.max(0, values.indexOf(PerlinLight.speed))

                snapMode: Slider.SnapAlways
                to: values.length - 1
                stepSize: 1
                from: 0
            }
            Slider {
                Layout.fillWidth: true

                property var values: [0.001, 0.0025, 0.005, 0.0075, 0.01, 0.025, 0.05]
                onValueChanged: PerlinLight.stretch = values[value]
                value: Math.max(0, values.indexOf(PerlinLight.stretch))

                snapMode: Slider.SnapAlways
                to: values.length - 1
                stepSize: 1
                from: 0
            }

            CGradientEdit {
                Layout.preferredHeight: 80
                Layout.fillWidth: true

                lightingGradient: PerlinLight.gradient
            }
        }
    }
}
