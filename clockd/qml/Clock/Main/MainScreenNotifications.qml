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

        sourceComponent: Frame {
            visible: alarm.nextTimeout.getTime() - AlarmService.now.getTime() < 15 * 60000
            contentItem: RowLayout {
                ColumnLayout {
                    spacing: -2

                    CLabel {
                        id: v
                        Layout.fillWidth: true
                        text: alarm.nextTimeout.toLocaleTimeString()
                        size: CLabel.Large
                    }
                    CLabel {
                        id: l
                        visible: alarm.name !== ''
                        Layout.fillWidth: true
                        size: CLabel.Medium
                        text: alarm.name
                        opacity: 0.72
                    }
                }
                CLabel {
                    Layout.alignment: Qt.AlignTop
                    text: {
                        const msecs = alarm.nextTimeout.getTime() - AlarmService.now.getTime()
                        return `in ${Theme.durationString(msecs)}`
                    }
                }
            }
        }

    }
}
