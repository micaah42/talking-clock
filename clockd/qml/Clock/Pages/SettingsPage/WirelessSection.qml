import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock.Style
import Clock.Controls
import NetworkManagerQml

Section {
    title: 'Wireless'
    icon: 'wifi'

    Component {
        id: pskDialog

        Dialog {
            id: dialog

            title: `Authentication Required for "${dialog.setting ? dialog.setting.ssid : ''}"`
            standardButtons: Dialog.Ok | Dialog.Cancel
            property WirelessNetwork network
            property WirelessSetting setting

            onAccepted: network.wirelessSetting = setting
            parent: Overlay.overlay
            x: (Overlay.overlay.width - width) / 2
            y: 32
            visible: true
            width: 420

            CTextField {
                Component.onCompleted: forceActiveFocus()
                onAccepted: dialog.accept()

                placeholderText: 'Password'
                onTextEdited: dialog.setting.psk = text
                text: dialog.setting ? dialog.setting.psk : ''
                width: parent.width
            }
        }
    }

    ListView {
        Layout.fillHeight: true
        Layout.fillWidth: true

        model: NetworkManagerQml.wirelessNetworks

        delegate: ItemDelegate {
            id: d
            property WirelessNetwork network: modelData
            width: ListView.view.width
            padding: 16

            onClicked: {
                const dialog = pskDialog.createObject(this)
                dialog.setting = network.newWirelessSettings(dialog)
                dialog.network = network
            }

            contentItem: RowLayout {
                spacing: 16
                Icon {
                    property bool locked: d.network.security !== WirelessNetwork.NoneSecurity
                    font.pixelSize: 24

                    text: {
                        if (d.network.signalStrength > 90)
                            return locked ? Icons.network_wifi_locked : Icons.network_wifi
                        if (d.network.signalStrength > 60)
                            return locked ? Icons.network_wifi_3_bar_locked : Icons.network_wifi_3_bar
                        if (d.network.signalStrength > 40)
                            return locked ? Icons.network_wifi_2_bar_locked : Icons.network_wifi_2_bar

                        return locked ? Icons.network_wifi_1_bar_locked : Icons.network_wifi_1_bar
                    }
                }
                CLabel {
                    text: d.network.ssid
                }
                Item {
                    Layout.preferredHeight: bttn.height
                    Layout.fillWidth: true
                }
                Button {
                    id: bttn
                    visible: d.network.wirelessSetting
                    font.family: Icons.fontFamily
                    text: Icons.settings
                }
            }
        }
    }
}
