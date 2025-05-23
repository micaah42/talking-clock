import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock
import Clock.Style
import Clock.Controls
import Clock.Alarms

Flickable {
    anchors.fill: parent
    contentHeight: column.implicitHeight + 16

    ColumnLayout {
        id: column
        width: parent.width

        Frame {
            Layout.fillWidth: true
            clip: true
            ActionDayItem {
                implicitHeight: contentHeight
                width: parent.width
            }
        }
        Frame {
            Layout.fillWidth: true
            clip: true
            NextAlarm {
                alarm: AlarmService.nextAlarm
                width: parent.width
            }
        }
    }
}
