import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14

import Clock 1.0
import Controls 1.0

Item {
    property var setting
    height: Material.delegateHeight

    Loader {
        width: parent.width
        active: setting["input"] === 0
        sourceComponent: CFontDialog {
            label: setting["key"]
            onValueChanged: SettingsService.setValue(setting["key"], value)
            Component.onCompleted: console.log('fontdialog', setting["key"])
        }
    }

    Loader {
        width: parent.width
        active: setting["input"] !== 0
        sourceComponent: CTextField {
            label: setting["key"]
            text: SettingsService.value(setting["key"])
            onTextChanged: SettingsService.setValue(setting["key"], text.toString())
        }
    }
}
