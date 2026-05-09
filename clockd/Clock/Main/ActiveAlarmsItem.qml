import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Clock

import "../Style"
import "../Alarms"
import "../Controls"

ColumnLayout {
    id: root
    spacing: 16

    function muteAlarms() {
        muteAlarmsRequested()
    }

    signal muteAlarmsRequested

    CLabel {
        Layout.fillWidth: true
        size: CLabel.XLarge
        text: `Active Alarms:`
    }

    Repeater {
        model: ActiveAlarmListModel

        delegate: AlarmNotification {
            id: d1
            Layout.fillWidth: true
            alarm: modelData

            Connections {
                target: root
                function onMuteAlarmsRequested() {
                    d1.muteAlarm()
                }
            }
        }
    }
    Item {
        Layout.fillWidth: true
    }
}
