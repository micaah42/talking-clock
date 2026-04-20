import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock
import "../Style"
import "../Controls"
import "../Alarms"

Flickable {
    readonly property AlarmListModel activeAlarms: AlarmListModel {}
    readonly property bool hasActiveAlarms: activeAlarms.length > 0
    contentHeight: column.implicitHeight + 16
    signal alarmAdded(var alarm)

    Component.onCompleted: console.log('@', SideBarConfiguration, SideBarConfiguration.availableSideBarSections)
    function muteAlarms() {
        for (var i = 0; i < repeater.count; i++) {
            const item = repeater.itemAt(i)
            if (item && item.muteAlarm)
                item.muteAlarm()
        }
    }

    ColumnLayout {
        id: column
        width: parent.width
        spacing: 16

        CFrame {
            Layout.fillWidth: true
            backgroundColor: Theme.accentDark
            visible: activeAlarms.length > 0

            contentItem: ColumnLayout {
                width: parent.width
                spacing: 16

                CLabel {
                    Layout.bottomMargin: 4
                    size: CLabel.XLarge
                    text: `Active Alarms:`
                }

                Repeater {
                    id: repeater
                    model: activeAlarms
                    delegate: AlarmNotification {
                        Layout.fillWidth: true
                        alarm: modelData
                    }
                }
            }
            background.data: BackgroundItem {
                text: Icons.notifications_active
            }
        }

        CFrame {
            Layout.fillWidth: true
            visible: chatbotItem.ollamaService.isAvailable
            clip: true

            contentItem: ChatbotItem {
                id: chatbotItem
                width: parent.width
            }
            background.data: BackgroundItem {
                text: Icons.robot_2
            }
        }

        CFrame {
            Layout.fillWidth: true
            clip: true

            contentItem: WeatherItem {
                id: weatherItem
                width: parent.width
            }
            background.data: BackgroundItem {
                text: weatherItem.currentSymbol
            }
        }

        CFrame {
            Layout.fillWidth: true
            clip: true
            contentItem: ActionDayItem {
                implicitHeight: contentHeight
                width: parent.width
            }

            background.data: BackgroundItem {
                text: Icons.calendar_month
            }
        }

        CFrame {
            Layout.fillWidth: true
            clip: true
            contentItem: NextAlarm {
                alarm: AlarmService.nextAlarm
                width: parent.width
            }

            background.data: BackgroundItem {
                text: Icons.alarm
            }
        }
    }

    component BackgroundItem: Icon {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: -implicitHeight / 4
        font.pixelSize: parent.width
        opacity: Theme.o11
    }

    Connections {
        target: AlarmService
        function onAlarmTriggered(alarm) {
            activeAlarms.append(alarm)
            alarmAdded(alarm)
        }
    }
}
