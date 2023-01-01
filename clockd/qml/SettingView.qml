import QtQuick 2.14
import QtQuick.Controls 2.14
import "qrc:/controls"

Item {
    property var setting
    height: childrenRect.height

    Loader {
        width: parent.width
        active: setting["options"].length > 0
        sourceComponent: CComboBox {
            label: setting["key"]
            model: setting["options"]
            text: settingsService.value(setting["key"])
            onActivated: {
                settingsService.setValue(setting["key"], model[index])
            }
        }
    }

    Loader {
        width: parent.width
        active: setting["options"].length === 0
        sourceComponent: CTextField {
            label: setting["key"]
            text: settingsService.value(setting["key"])
            onTextChanged: {
                settingsService.setValue(setting["key"], text.toString());
            }
        }
    }
}
