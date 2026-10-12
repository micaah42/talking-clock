import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Clock

import "../Style"
import "../Alarms"
import "../Controls"

StackLayout {
    id: root
    readonly property AlarmListModel activeAlarms: ActiveAlarmListModel
    readonly property bool hasActiveAlarms: activeAlarms.length > 0
    currentIndex: hasActiveAlarms ? 1 : 0

    signal alarmAdded(var alarm)

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
            page: ChatbotItem {}
        }
    ]

    ColumnLayout {
        spacing: 8

        RowLayout {
            TabBar {
                id: tabBar
                Layout.leftMargin: 8
                Material.background: 'transparent'
                currentIndex: swipeView.currentIndex

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
                        onClicked: swipeView.currentIndex = index
                    }
                }
            }

            Item {
                Layout.fillWidth: true
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
                spacing: 32

                Repeater {
                    model: pages
                    delegate: FeedbackLoader {
                        required property SideBarPage modelData
                        required property int index

                        visible: Math.abs(index - swipeView.currentIndex) <= 1
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
        }
    }

    CFrame {
        contentItem: ActiveAlarmsItem {}
    }

    Connections {
        target: AlarmService
        function onAlarmTriggered(alarm) {
            activeAlarms.append(alarm)
            alarmAdded(alarm)
        }
    }
}
