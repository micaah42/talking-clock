import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import Clock
import "../Style"
import "../Controls"
import "../Lighting"

ColumnLayout {
    readonly property var modeScreens: [//@
        [staticLightComponent, Lighting.staticLight], //@
        [wavingLightComponent, Lighting.wavingLight], //@
        [monoRotationLightComponent, Lighting.monoRotationLight] //@
    ]

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

    ButtonGroup {
        id: group
        buttons: buttonsRow

        property int currentIndex: modeScreens.findIndex(x => x[1] === Lighting.mode)
        checkedButton: currentIndex === -1 ? null : buttons[currentIndex] || null
        onCurrentIndexChanged: swipeView.replace(null, modeScreens[currentIndex][0])
    }

    RowLayout {
        id: buttonsRow
        enabled: Lighting.enabled

        Repeater {
            model: modeScreens
            delegate: Button {
                Layout.fillWidth: true
                implicitWidth: 0

                highlighted: modelData[1] === Lighting.mode
                onClicked: Lighting.mode = modelData[1]
                text: modelData[1].name
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

    Dialog {
        id: customColorPopup
        anchors.centerIn: Overlay.overlay
        height: 7 * window.height / 8
        width: 7 * window.width / 8

        title: 'Custom Color'
        contentItem: ColorPicker {
            id: customColorPicker
            onCurrentColorChanged: valueEdited(currentColor)
            value: value
        }
    }

    Component {
        id: staticLightComponent
        Item {
            ColorPicker {
                anchors.fill: parent
                onCurrentColorChanged: Lighting.staticLight.color = currentColor
                value: Lighting.staticLight
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
                    onClicked: Lighting.monoRotationLight.colors = modelData.colors
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
                    spinBox.onValueChanged: Lighting.wavingLight.length = spinBox.value
                    spinBox.value: Lighting.wavingLight.length
                    spinBox.stepSize: 5
                    spinBox.from: 5
                    spinBox.to: 250
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
