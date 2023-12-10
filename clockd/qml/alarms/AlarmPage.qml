import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQuick.Shapes 1.15

import "."
import ".."
import Clock 1.0
import Controls 1.0

Item {
    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 16

        NextAlarm {
            Layout.preferredWidth: 3 * parent.width / 7
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop
            padding: 16
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: view
                anchors.margins: 5
                anchors.fill: parent
                spacing: 5
                clip: true

                model: AlarmService.model
                delegate: AlarmItem {
                    width: view.width
                }

                footer: Button {
                    width: parent.width
                    text: "New Alarm"
                    onClicked: {
                        AlarmService.model.push()
                    }
                    font.family: FontService.family
                }
            }
        }
    }
}
