import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import Clock
import Clock.Style
import Clock.Controls
import Clock.Pages.Lighting

ColumnLayout {
    readonly property var modeScreens: [//@
        [staticLightComponent, Lighting.staticLight], //@
        [wavingLightComponent, Lighting.wavingLight], //@
        [monoRotationLightComponent, Lighting.monoRotationLight] //@
    ]

    Component.onCompleted: {
        for (let x of modeScreens)
            if (x[1] === Lighting.mode)
                swipeView.push(x[0])
    }

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
            model: modeScreens
            delegate: Button {
                Layout.fillWidth: true
                highlighted: modelData[1] === Lighting.mode
                text: modelData[1].name
                implicitWidth: 0
                onClicked: {
                    Lighting.mode = modelData[1]
                    swipeView.replace(modelData[0])
                }
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
                id: swipeView
                anchors.fill: parent
                anchors.margins: 8
                initialItem: Item {}
                enabled: Lighting.enabled
                spacing: 32
                clip: true
            }
        }
    }

    Component {
        id: staticLightComponent
        PalettePicker {
            onValueEdited: x => Lighting.staticLight.color = x
            value: Lighting.staticLight
        }
    }

    Component {
        id: monoRotationLightComponent
        ListView {
            model: Palettes.palettes
            delegate: ItemDelegate {
                onClicked: Lighting.monoRotationLight.colors = modelData.colors
                width: ListView.view.width
                contentItem: ColumnLayout {
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
        }
    }
    Component {
        id: wavingLightComponent
        RowLayout {
            spacing: 16

            ColorPicker {
                onCurrentColorChanged: Lighting.wavingLight.a = currentColor
                value: Lighting.wavingLight.a
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            ColorPicker {
                onCurrentColorChanged: Lighting.wavingLight.b = currentColor
                value: Lighting.wavingLight.b
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            ColumnLayout {
                CSpinBox {
                    labelText: 'Length'
                    spinBox {
                        onValueChanged: Lighting.wavingLight.length = spinBox.value
                        value: Lighting.wavingLight.length
                        stepSize: 5
                        from: 5
                        to: 250
                    }
                }
                CSpinBox {
                    labelText: 'Speed'
                    spinBox.onValueChanged: Lighting.wavingLight.speed = spinBox.value / 10
                    spinBox.value: 10 * Lighting.wavingLight.speed
                    spinBox.from: 1
                    spinBox.to: 25
                }
                Item {
                    Layout.fillHeight: true
                }
            }
        }
    }
}
