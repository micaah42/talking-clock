import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock
import Clock.Style
import Clock.Controls

ColumnLayout {
    property Alarm alarm: AlarmService.nextAlarm

    Loader {
        Layout.fillWidth: true
        active: alarm !== null

        sourceComponent: CFrame {
            visible: alarm.nextTimeout.getTime() - AlarmService.now.getTime() < 15 * 60000
            contentItem: RowLayout {
                ColumnLayout {
                    spacing: -2

                    CLabel {
                        id: v
                        Layout.fillWidth: true
                        text: alarm.nextTimeout.toLocaleTimeString()
                        size: CLabel.XLarge
                    }
                    CLabel {
                        id: l
                        visible: alarm.name !== ''
                        Layout.fillWidth: true
                        size: CLabel.Large
                        opacity: Theme.o72
                        text: alarm.name
                    }
                }
                CLabel {
                    size: CLabel.Large
                    text: {
                        const msecs = alarm.nextTimeout.getTime() - AlarmService.now.getTime()
                        return `in ${Theme.durationString(msecs)}`
                    }
                }
            }
        }
    }
}
