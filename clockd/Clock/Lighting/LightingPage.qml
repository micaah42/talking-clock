import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Effects

import Clock
import QLighting

import "../Style"
import "../Controls"
import "../Lighting"

ColumnLayout {
    spacing: 8

    RowLayout {
        Layout.minimumHeight: 48
        Layout.maximumHeight: 48
        spacing: 8

        CButton {
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

    // RowLayout {
    //     id: buttonsRow
    //     enabled: Lighting.enabled

    //     Repeater {
    //         model: LightingInit.modes
    //         delegate: CButton {
    //             property LightMode lightMode: modelData
    //             Layout.fillWidth: true
    //             implicitWidth: 0

    //             onClicked: LightingInit.mode = lightMode
    //             checked: lightMode === LightingInit.mode
    //             highlighted: checked
    //             text: lightMode.name
    //         }
    //     }
    // }
    Rectangle {
        Layout.preferredHeight: 24
        Layout.fillWidth: true
        border.color: Material.foreground
        color: Material.background
        radius: 12
        clip: true

        Item {
            id: sourceItem
            anchors.fill: parent

            LightingDisplay {
                anchors.centerIn: parent
                width: sourceItem.width - 16
                lighting: Lighting
                spacing: 0
                height: 4
                radius: 1
            }
        }

        MultiEffect {
            source: sourceItem
            anchors.fill: sourceItem
            blurEnabled: true
            blurMultiplier: 32
            blurMax: 16
            blur: 4.
        }
    }

    Item {
        Layout.fillHeight: true
        Layout.fillWidth: true

        CFrame {
            anchors.fill: parent
            topPadding: 24

            GridLayout {
                anchors.fill: parent
                columnSpacing: 16
                rowSpacing: 16
                columns: 4

                CButton {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    implicitWidth: 0
                    font.family: Icons.fontFamily
                    text: Icons.chevron_left
                    flat: true
                }
                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.columnSpan: 2

                    CLabel {
                        anchors.centerIn: parent
                        size: CLabel.Large
                        text: 'Hey'
                    }
                }
                CButton {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    implicitWidth: 0
                    font.family: Icons.fontFamily
                    text: Icons.chevron_right
                    flat: true
                }

                Repeater {
                    model: LightingInit.modes
                    delegate: CButton {
                        id: d
                        property LightMode mode: modelData
                        property bool wasHeld: false
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        implicitWidth: 0
                        text: mode.name
                        bottomInset: 0
                        topInset: 0

                        onPressAndHold: {
                            wasHeld = true
                            tt.open()
                        }
                        onCanceled: wasHeld = false
                        onClicked: {
                            if (wasHeld) {
                                wasHeld = false
                                return
                            }
                            LightingInit.mode = mode
                        }

                        ToolTip {
                            id: tt
                            width: 256

                            contentItem: GridLayout {
                                columns: 2

                                LightingDisplay {
                                    Layout.fillWidth: true
                                    Layout.columnSpan: 2
                                    implicitHeight: 16
                                    lighting: LightingBase {
                                        enabled: true
                                        mode: d.mode
                                    }
                                }

                                Repeater {
                                    function isSimpleType(value) {
                                        const type = typeof value
                                        if (type === 'string')
                                            return true
                                        if (type === 'number')
                                            return true

                                        return false
                                    }

                                    model: {
                                        var entries = Object.entries(d.mode)
                                        entries = entries.filter(x => x[0] !== 'objectName')
                                        entries = entries.filter(x => isSimpleType(x[1]))
                                        return entries.reduce((acc, x) => acc.concat(x), [])
                                    }
                                    delegate: CLabel {
                                        Layout.fillWidth: true
                                        text: modelData
                                    }
                                }
                            }
                        }
                    }
                }
            }


            /*
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

                onCompChanged: replace(null, comp)
                initialItem: Item {}
            }
            */
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
            model: LightingGradient.NumPresets
            delegate: ItemDelegate {
                id: d
                width: ListView.view.width

                property LightingGradient gradient: LightingGradient {
                    preset: index || 0
                }

                onClicked: MonoRotationLight.gradient = gradient
                text: gradient.presetName(modelData)

                Rectangle {
                    width: parent.width
                    height: 16

                    Component {
                        id: gradientStop
                        GradientStop {}
                    }

                    gradient: Gradient {
                        property LightingGradient lightingGradient: d.gradient
                        orientation: Gradient.Horizontal

                        stops: {
                            const model = Array.from(lightingGradient.gradientStops)
                            return model.map(stop => gradientStop.createObject(this, stop))
                        }
                    }
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
