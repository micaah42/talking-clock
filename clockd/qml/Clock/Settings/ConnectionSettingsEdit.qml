import QtQuick
import QtQuick.Layouts

import Clock.Style
import Clock.Controls
import NetworkManagerQml

ColumnLayout {
    property WirelessSetting wirelessSetting

    CLabel {
        text: wirelessSettings.ssid
    }
    CTextField {
        placeholderText: 'PSK'
        onTextEdited: wirelessSetting.psk = text
        text: wirelessSetting.psk
    }
}
