import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock.Style
import Clock.Controls
import NetworkManagerQml

Section {
    id: root
    title: 'Wireless'
    icon: 'wifi'

    property WirelessDevice device: NetworkManagerQml.wirelessDevice
    property bool showScanning: false
    extraHeaderContent: RowLayout {
        BusyIndicator {
            id: busyIndicator
            Layout.preferredHeight: switchItem.height
            opacity: 0

            SequentialAnimation {
                running: showScanning
                onStarted: busyIndicator.opacity = 1
                onFinished: showScanning = false
                PauseAnimation {
                    duration: 3000
                }
                OpacityAnimator {
                    target: busyIndicator
                    duration: 1000
                    from: 1
                    to: 0
                }
            }
        }

        Switch {
            id: switchItem
            onCheckableChanged: NetworkManagerQml.wirelessEnabled = checked
            checked: NetworkManagerQml.wirelessEnabled
        }
    }

    Component {
        id: pskDialog

        Dialog {
            id: dialog

            title: `Authentication Required for "${dialog.wirelessSetting ? dialog.wirelessSetting.ssid : ''}"`
            standardButtons: Dialog.Ok | Dialog.Cancel

            readonly property WirelessSecuritySetting wirelessSecuritySetting: {
                return settings.setting(Setting.WirelessSecurity)
            }
            readonly property WirelessSetting wirelessSetting: {
                return settings.setting(Setting.Wireless)
            }
            property Settings settings

            onClosed: this.destroy()
            onAccepted: NetworkManagerQml.addConnection(settings)
            parent: Overlay.overlay
            x: (Overlay.overlay.width - width) / 2
            y: 32
            visible: true
            width: 420

            CTextField {
                Component.onCompleted: forceActiveFocus()
                onAccepted: dialog.accept()

                placeholderText: 'Password'
                onTextEdited: dialog.wirelessSecuritySetting.psk = text
                text: dialog.setting ? dialog.wirelessSecuritySetting.psk : ''
                width: parent.width
            }
        }
    }

    Component {
        id: connectionDialog

        Dialog {
            id: dialog

            property WirelessNetwork network
            property Connection connection

            anchors.centerIn: Overlay.overlay
            height: window.contentItem.height - 64
            width: window.contentItem.width - 64

            standardButtons: Dialog.Ok | Dialog.Cancel
            title: connection?.name || ''
            onClosed: this.destroy()
            visible: true

            contentItem: ConnectionSettingsEdit {
                connection: dialog.connection
                network: dialog.network
            }
        }
    }

    function signalStringthToIcon(strength, locked) {
        if (strength > 80)
            return locked ? Icons.network_wifi_locked : Icons.network_wifi
        if (strength > 50)
            return locked ? Icons.network_wifi_3_bar_locked : Icons.network_wifi_3_bar
        if (strength > 30)
            return locked ? Icons.network_wifi_2_bar_locked : Icons.network_wifi_2_bar
        return locked ? Icons.network_wifi_1_bar_locked : Icons.network_wifi_1_bar
    }

    RowLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true

            property bool scanRequested: false
            onContentYChanged: {
                if (contentY < -32)
                    scanRequested = true
            }
            onFlickEnded: {
                if (scanRequested) {
                    scanRequested = false
                    showScanning = true
                    device.scan()
                }
            }

            model: NetworkManagerQml.wirelessDevice?.wirelessNetworks || 0

            delegate: ItemDelegate {
                id: d
                property WirelessNetwork network: modelData
                width: ListView.view.width
                padding: 16

                onClicked: {
                    if (network.connection) {
                        NetworkManagerQml.activateConnection(network.connection)
                        return
                    }

                    const dialog = pskDialog.createObject(this)
                    dialog.settings = network.newSettings(dialog)
                }

                contentItem: RowLayout {
                    spacing: 16
                    Icon {
                        property bool locked: d.network.security !== WirelessNetwork.NoneSecurity
                        text: signalStringthToIcon(d.network.signalStrength, locked)
                        font.pixelSize: 24
                    }
                    CLabel {
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                        text: d.network.ssid
                    }
                    CLabel {
                        visible: device.activeNetwork === d.network
                        text: 'Connected'
                    }

                    Button {
                        id: bttn
                        visible: d.network.connection
                        font.family: Icons.fontFamily
                        text: Icons.settings

                        onClicked: {
                            const dialog = connectionDialog.createObject(this)
                            dialog.connection = d.network.connection
                            dialog.network = d.network
                        }
                    }
                }
            }
        }
    }
}
