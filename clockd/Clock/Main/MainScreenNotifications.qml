import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock
import NetworkManagerQml

import "../Style"
import "../Controls"

ColumnLayout {
    property Alarm alarm: AlarmService.nextAlarm

    Loader {
        visible: alarm && alarm.nextTimeout.getTime() - AlarmService.now.getTime() < 15 * 60000
        Layout.fillWidth: true
        active: alarm !== null

        sourceComponent: CFrame {
            contentItem: SimpleListItem {
                property bool isUntitled: alarm.name === ''
                descriptionItem.opacity: isUntitled ? Theme.o56 : Theme.o72
                description: isUntitled ? '(Untitled Alarm)' : alarm.name

                icon: Icons.alarm
                title: {
                    const msecs = alarm.nextTimeout.getTime() - AlarmService.now.getTime()
                    return `Alarm at ${Theme.timeString(alarm.nextTimeout)}, in ${Theme.durationString(msecs)}`
                }
            }
        }
    }

    CFrame {
        Layout.fillWidth: true
        opacity: NetworkManagerQml.connectivity === 4 ? connChangedTimer.running : 1

        Behavior on opacity {
            OpacityAnimator {}
        }

        contentItem: SimpleListItem {
            id: connectivity
            descriptionItem.wrapMode: Text.Wrap

            property int con: {
                switch (NetworkManagerQml.connectivity) {
                case 0:
                    icon = Icons.cloud_sync
                    title = 'Unknown Connectivity'
                    description = 'Network connectivity is unknown.'
                    break
                case 1:
                    icon = Icons.cloud_alert
                    title = 'No Connectivity'
                    description = 'The host is not connected to any network.'
                    break
                case 2:
                    icon = Icons.captive_portal
                    title = 'Portal'
                    description = 'The host is behind a captive portal and cannot reach the full Internet.'
                    break
                case 3:
                    icon = Icons.cloud_alert
                    title = 'Limited'
                    description = 'The host is connected to a network, but does not appear to be able to reach the full Internet.'
                    break
                case 4:
                    icon = Icons.cloud_done
                    title = 'Full'
                    description = 'The host is connected to a network, and appears to be able to reach the full Internet.'
                    break
                }

                connChangedTimer.start()
                return NetworkManagerQml.connectivity
            }

            Timer {
                id: connChangedTimer
                interval: 15000
            }
        }
    }
}
