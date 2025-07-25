import QtQuick 
import QtQuick.Layouts 1.14
import QtQuick.Controls 
import "qrc:/controls"

Item {
    ColumnLayout {
        anchors.margins: 5
        anchors.fill: parent

        Repeater {
            model: taskService.processes
            delegate: DarkCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                CLabel {
                    anchors.margins: 5
                    anchors.fill: parent
                    text: JSON.stringify(modelData).replace(',', '\n')
                }
            }
        }
    }
}
