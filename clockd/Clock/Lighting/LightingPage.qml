import QtQuick
import QtQuick.Layouts
import QtQuick.Effects
import QtQuick.Controls.Material
import QtQuick.Window

import Clock
import QLighting

import "../Style"
import "../Controls"
import "../Components"
import "../Lighting"

ColumnLayout {
    id: root
    spacing: 8
    property Window window: Window.window

    Item {
        Layout.preferredHeight: 24
        Layout.fillWidth: true
        //  border.color: Material.foreground
        //  color: Material.background
        //  radius: 12
        clip: true

        Item {
            id: sourceItem
            anchors.fill: parent

            // layer.enabled: true
            LightingDisplay {
                anchors.centerIn: parent
                width: sourceItem.width
                height: 24
                spacing: 1
                radius: 1

                lighting: LightingBase {
                    mode: Lighting.mode
                    enabled: true
                    leds: 48
                }
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
    Item {
        Layout.fillHeight: true
        Layout.fillWidth: true

        CFrame {
            anchors.fill: parent
            topPadding: 24

            GridLayout {
                id: grid

                readonly property int modesPerPage: (rows - 1) * columns
                readonly property int modePages: Math.ceil(LightingInit.modes.length / modesPerPage)
                property int index: 0
                anchors.fill: parent
                columnSpacing: 16
                rowSpacing: 16
                columns: 3
                rows: 4

                CButton {
                    Layout.fillWidth: true
                    implicitWidth: 0
                    onClicked: grid.index = (grid.index - 1 + grid.modePages) % grid.modePages
                    font.family: Icons.fontFamily
                    text: Icons.chevron_left
                    // flat: true
                }
                Item {
                    Layout.fillWidth: true

                    CLabel {
                        anchors.centerIn: parent
                        text: `${grid.index + 1} / ${grid.modePages}`
                        size: CLabel.XLarge
                    }
                }
                CButton {
                    Layout.fillWidth: true
                    implicitWidth: 0
                    onClicked: grid.index = (grid.index + 1) % grid.modePages
                    font.family: Icons.fontFamily
                    text: Icons.chevron_right
                    // flat: true
                }

                Repeater {
                    model: {
                        const start = grid.index * grid.modesPerPage
                        const modes = LightingInit.modes.slice(start, start + grid.modesPerPage)
                        const filler = Array(grid.modesPerPage - modes.length).fill(null)
                        return modes.concat(filler)
                    }

                    delegate: Loader {
                        id: d

                        property LightMode mode: modelData

                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        active: mode

                        sourceComponent: Item {
                            LightingDisplay {
                                anchors.fill: parent
                                spacing: -3
                                radius: 3

                                lighting: LightingBase {
                                    mode: d.mode
                                    enabled: true
                                    leds: 32
                                }
                            }

                            CButton {
                                property bool wasHeld: false

                                anchors.fill: parent
                                text: d.mode.name
                                bottomInset: 0
                                topInset: 0
                                flat: true

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
                                    LightingInit.mode = d.mode
                                }
                            }

                            Dialog {
                                id: tt

                                anchors.centerIn: parent
                                parent: root.window?.contentItem || null
                                height: parent.height - 64
                                width: parent.width - 64
                                modal: true

                                title: d.mode.name

                                contentItem: LightingModeEdit {
                                    lightMode: d.mode
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
