import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import Clock 1.0

Item {
    GridLayout {
        anchors.margins: 15
        anchors.fill: parent
        columnSpacing: 10
        rowSpacing: 10
        columns: 4

        // rows: 10
        Repeater {
            model: SettingsService.settings
            delegate: SettingView {
                setting: modelData
                Layout.fillWidth: true
            }
        }

        Item {
            Layout.fillWidth: true
        }
        Item {
            Layout.fillHeight: true
        }
    }
}
