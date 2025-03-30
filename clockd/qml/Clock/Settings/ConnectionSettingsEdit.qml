import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock.Style
import Clock.Controls
import NetworkManagerQml

ColumnLayout {

    readonly property WirelessSecuritySetting wirelessSecuritySetting: {
        return connection?.settings?.setting(Setting.WirelessSecurity) || null
    }
    readonly property WirelessSetting wirelessSetting: {
        return connection?.settings?.setting(Setting.Wireless) || null
    }

    property WirelessNetwork network
    property Connection connection
    spacing: 16

    onConnectionChanged: {
        if (connection)
            connection.fetchSecrets()
    }

    TabBar {
        id: bar
        Layout.fillWidth: true

        Repeater {
            id: repeater
            model: connection ? Array.from(connection.settings.settings).map(setting => setting.type) : []
            delegate: TabButton {
                Component.onCompleted: console.log(connection, text)
                text: {
                    switch (modelData) {
                    case Setting.Wireless:
                        return 'Wireless'
                    case Setting.WirelessSecurity:
                        return 'Wireless Security'
                    }
                }
            }
        }
    }

    Loader {
        Layout.fillHeight: true
        Layout.fillWidth: true
        sourceComponent: {
            switch (repeater.model[bar.currentIndex]) {
            case Setting.Wireless:
                return wirelessSettingEdit
            case Setting.WirelessSecurity:
                return wirelessSecuritySettingEdit
            }
        }
    }

    Component {
        id: wirelessSettingEdit
        ColumnLayout {
            spacing: 8
            CTextField {
                placeholderText: 'SSID'
                text: wirelessSetting?.ssid || ''
                readOnly: true
            }
            CComboBox {
                placeholderText: 'BSSID'
                text: wirelessSetting?.bssid || ''
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }
    Component {
        id: wirelessSecuritySettingEdit
        ColumnLayout {
            spacing: 8
            ComboBox {
                Layout.preferredWidth: pskField.width
                currentIndex: securities.indexOf(network?.security || WirelessNetwork.NoneSecurity)

                property var securities: [//@
                    WirelessNetwork.NoneSecurity, //@
                    WirelessNetwork.StaticWep, //@
                    WirelessNetwork.DynamicWep, //@
                    WirelessNetwork.Leap, //@
                    WirelessNetwork.WpaPsk, //@
                    WirelessNetwork.WpaEap, //@
                    WirelessNetwork.Wpa2Psk, //@
                    WirelessNetwork.Wpa2Eap, //@
                    WirelessNetwork.SAE, //@
                    WirelessNetwork.Wpa3SuiteB192, //@
                    WirelessNetwork.OWE //@
                ]
                model: securities.map(function (security) {
                    switch (security) {
                    case WirelessNetwork.NoneSecurity:
                        return "NoneSecurity"
                    case WirelessNetwork.StaticWep:
                        return "StaticWep"
                    case WirelessNetwork.DynamicWep:
                        return "DynamicWep"
                    case WirelessNetwork.Leap:
                        return "Leap"
                    case WirelessNetwork.WpaPsk:
                        return "WpaPsk"
                    case WirelessNetwork.WpaEap:
                        return "WpaEap"
                    case WirelessNetwork.Wpa2Psk:
                        return "Wpa2Psk"
                    case WirelessNetwork.Wpa2Eap:
                        return "Wpa2Eap"
                    case WirelessNetwork.SAE:
                        return "SAE"
                    case WirelessNetwork.Wpa3SuiteB192:
                        return "SuiteB192"
                    case WirelessNetwork.OW:
                        return "OW"
                    }
                })
            }

            CTextField {
                id: pskField
                placeholderText: 'PSK'
                onTextEdited: wirelessSecuritySetting.psk = text
                text: wirelessSecuritySetting?.psk || ''
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }
}
