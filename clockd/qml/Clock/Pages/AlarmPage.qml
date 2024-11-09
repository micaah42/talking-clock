import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQuick.Shapes 1.15

import Clock 1.0
import Clock.Controls 1.0
import Clock.Pages.AlarmPage 1.0

Item {
    RowLayout {
        anchors.fill: parent

        NextAlarm {
            Layout.maximumWidth: 0.3 * parent.width
            Layout.fillHeight: true
            Layout.margins: 8
            alarm: AlarmService.nextAlarm
        }

        Rectangle {
            Layout.preferredWidth: 1.5
            Layout.fillHeight: true
            Layout.margins: 8
            radius: width / 2
            opacity: 0.5
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                id: view

                anchors.fill: parent
                spacing: 5
                clip: true

                model: AlarmService.model

                delegate: AlarmItem {
                    alarm: model.value
                    width: view.width
                }

                header: Button {
                    width: parent.width
                    height: 64

                    Material.roundedScale: Material.ExtraSmallScale
                    font.family: FontService.family
                    font.pixelSize: 24

                    onClicked: AlarmService.newAlarm()
                    text: "New Alarm"
                }
            }
        }
    }
}
