import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

Item {
    Card {
        anchors.margins: 10
        anchors.fill: parent

        GridLayout {
            anchors.margins: 15
            anchors.fill: parent
            columnSpacing: 10

            columns: 4
            // rows: 10

            Repeater {
                model: settingsService.settings
                delegate: SettingView {
                    setting: modelData
                    Layout.fillWidth: true
                    // Layout.preferredHeight: implicitHeight
                }
            }

            Item { Layout.fillWidth: true }
            Item { Layout.fillHeight: true }
        }
    }
}