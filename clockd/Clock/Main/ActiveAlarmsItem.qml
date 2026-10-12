import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Clock

import "../Style"
import "../Alarms"
import "../Controls"
import "../Components"

ColumnLayout {
    id: root
    spacing: 8

    function muteAlarms() {
        muteAlarmsRequested()
    }

    signal muteAlarmsRequested

    CLabel {
        Layout.fillWidth: true
        size: CLabel.XLarge
        text: `Active Alarms:`
    }

    FlickableColumnLayout {
        Layout.preferredHeight: Math.min(contentHeight, root.height / 2)
        Layout.fillWidth: true

        Repeater {
            model: ActiveAlarmListModel

            delegate: AlarmNotification {
                id: d1
                Layout.fillWidth: true
                alarm: modelData

                Connections {
                    target: ActiveAlarmListModel
                    function onMuteAlarmsRequested() {
                        d1.muteAlarm()
                    }
                }
            }
        }
    }

    Divider {
        Layout.fillWidth: true
    }

    ChatBotResponseItem {
        Layout.fillHeight: true
        Layout.fillWidth: true
        response: ChatBotResponseService.alarmResponse
    }
}
