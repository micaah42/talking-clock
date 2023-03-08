import QtQuick 2.14
import QtQuick.Controls 2.14
import "qrc:/controls"
import Clock 1.0

Item {
    property var setting
    height: childrenRect.height

    Loader {
        width: parent.width
        active: setting["options"].length > 0
        sourceComponent: CComboBox {
            label: setting["key"]
            model: setting["options"]
            text: SettingsService.value(setting["key"])
            onActivated: {
                SettingsService.setValue(setting["key"], model[index])
            }
        }
    }

    Loader {
        width: parent.width
        active: setting["options"].length === 0
        sourceComponent: CTextField {
            label: setting["key"]
            text: SettingsService.value(setting["key"])
            onTextChanged: {
                SettingsService.setValue(setting["key"], text.toString())
            }
        }
    }
}
