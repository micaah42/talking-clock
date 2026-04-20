import QtQuick
import QtQuick.Layouts
import Clock

ListView {
    id: root

    readonly property bool hasActiveAlarms: model.count > 1
    onHasActiveAlarmsChanged: console.log('has active alarms:', hasActiveAlarms)

    // readonly property alias count: model.count
    spacing: 16

    function clearAlarms() {
        model.remove(0, model.count - 1)
    }

    function muteAlarms() {
        for (var i = 0; i < model.count; i++) {
            const item = model.get(i)
            if (item && item.muteAlarm)
                item.muteAlarm()
        }
    }

    model: ObjectModel {
        id: model
        CFrame {
            width: root.width
            clip: true

            ActionDayItem {
                implicitHeight: contentHeight
                width: parent.width
            }
        }
    }

    Component {
        id: alarmNotification
        CFrame {
            property alias alarm: notification.alarm
            function muteAlarm() {
                notification.muteAlarm()
            }

            width: root.width
            clip: true

            AlarmNotification {
                id: notification
                width: parent.width
            }
        }
    }

    Connections {
        target: AlarmService
        function onAlarmTriggered(alarm) {
            const properties = {
                "alarm": alarm
            }

            const item = alarmNotification.createObject(null, properties)
            model.insert(0, item)
        }
    }
}
