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
    readonly property AlarmListModel activeAlarms: ActiveAlarmListModel
    readonly property bool hasActiveAlarms: activeAlarms.length > 0

    signal alarmAdded(var alarm)
    spacing: 8

    function muteAlarms() {
        muteAlarmsRequested()
    }

    signal muteAlarmsRequested

    component SideBarPage: QtObject {
        property Component page
        property string title
        property string name
        property string icon
    }

    readonly property list<SideBarPage> pages: [
        SideBarPage {
            title: "Weather"
            icon: WeatherService.currentSymbol
            page: WeatherItem {}
        },
        SideBarPage {
            title: "Action Days"
            icon: Icons.calendar_month
            page: ActionDayItem {}
        },
        SideBarPage {
            title: "Upcoming Alarms"
            icon: Icons.alarm

            page: NextAlarmItem {
                alarm: AlarmService.nextAlarm
            }
        },
        SideBarPage {
            title: "Chatbot"
            icon: Icons.robot_2
            page: Flickable {
                contentHeight: item.implicitHeight
                ChatbotItem {
                    id: item
                    width: parent.width
                }
            }
        }
    ]

    SideBarPage {
        id: activeAlarmsPage
        title: "Active Alarms!"
        icon: Icons.alarm
        page: ActiveAlarmsItem {
            id: activeAlarmsItem

            Connections {
                target: root
                function onMuteAlarmsRequested() {
                    activeAlarmsItem.muteAlarms()
                }
            }
        }
    }

    RowLayout {
        TabBar {
            id: tabBar
            Layout.leftMargin: 8
            Material.background: 'transparent'

            Repeater {
                id: tabButtonRepeater
                model: pages
                delegate: TabButton {
                    id: d
                    Material.background: 'transparent'
                    implicitWidth: height
                    font.pixelSize: Theme.fontSizeXLarge
                    font.family: Icons.fontFamily
                    text: modelData.icon
                }
            }
        }

        Item {
            Layout.fillWidth: true
        }

        Icon {
            visible: root.hasActiveAlarms
            Layout.preferredWidth: height
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: Theme.fontSizeXLarge
            text: Icons.priority_high

            background: Rectangle {
                color: Theme.accent
                radius: height / 2
            }
        }
    }

    CFrame {
        id: frame
        Layout.fillHeight: true
        Layout.fillWidth: true
        padding: 0

        contentItem: SwipeView {
            id: swipeView

            visible: !hasActiveAlarms
            currentIndex: tabBar.currentIndex
            spacing: 32

            onCurrentIndexChanged: {
                if (tabBar.currentIndex !== currentIndex)
                    tabBar.currentIndex = currentIndex
            }

            Repeater {
                model: pages
                delegate: FeedbackLoader {
                    required property SideBarPage modelData
                    required property int index

                    active: Math.abs(index - swipeView.currentIndex) <= 1
                    sourceComponent: modelData.page

                    Item {
                        anchors.fill: parent
                        anchors.rightMargin: -frame.rightPadding
                        anchors.leftMargin: -frame.leftPadding
                        anchors.margins: -frame.padding
                        clip: true

                        Icon {
                            anchors.bottom: parent.bottom
                            anchors.right: parent.right
                            anchors.margins: -implicitHeight / 4
                            font.pixelSize: parent.width
                            opacity: Theme.o11
                            text: modelData.icon
                        }
                    }
                }
            }
        }

        FeedbackLoader {
            anchors.fill: parent
            visible: hasActiveAlarms
            active: hasActiveAlarms
            sourceComponent: activeAlarmsPage.page
        }
    }

    Connections {
        target: AlarmService
        function onAlarmTriggered(alarm) {
            activeAlarms.append(alarm)
            alarmAdded(alarm)
        }
    }
}
