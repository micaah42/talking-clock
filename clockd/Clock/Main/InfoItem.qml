import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock
import "../Style"
import "../Controls"
import "../Alarms"

Flickable {
    anchors.fill: parent
    contentHeight: column.implicitHeight + 16

    ColumnLayout {
        id: column

        width: parent.width
        spacing: 16

        CFrame {
            Layout.fillWidth: true
            clip: true
            ActionDayItem {
                implicitHeight: contentHeight
                width: parent.width
            }
        }
        CFrame {
            Layout.fillWidth: true
            clip: true
            NextAlarm {
                alarm: AlarmService.nextAlarm
                width: parent.width
            }
        }
    }
}
