import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQuick.Shapes 1.15

import Clock 1.0
import Clock.Style
import Clock.Controls 1.0
import Clock.Pages.AlarmPage 1.0

Item {
    RowLayout {
        anchors.fill: parent

        NextAlarm {
            Layout.maximumWidth: 0.3 * parent.width
            Layout.fillHeight: true
            Layout.margins: 8
            alarm: AlarmService.nextAlarm
        }

        Rectangle {
            Layout.preferredWidth: 1.5
            Layout.fillHeight: true
            Layout.margins: 8
            radius: width / 2
            opacity: 0.5
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                id: view

                anchors.fill: parent
                spacing: 5
                clip: true

                model: AlarmService.model

                delegate: AlarmItem {
                    alarm: model.value
                    width: view.width
                }

                header: RowLayout {
                    width: parent.width
                    height: 56

                    Button {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        font.pixelSize: 24
                        text: "New Alarm"
                        onClicked: AlarmService.newAlarm()
                    }

                    Button {
                        Layout.preferredWidth: height
                        Layout.fillHeight: true
                        font.pixelSize: 24
                        font.family: Icons.fontFamily
                        text: Icons.more_vert
                        onClicked: menu.open()

                        Menu {
                            id: menu
                            x: parent.width - width
                            y: parent.height + 8
                            MenuItem {
                                text: '1 Minute'
                                onClicked: {
                                    const alarm = AlarmService.newAlarm()
                                    alarm.time = new Date(new Date().getTime() + 60000)
                                    alarm.name = '1 Minute from now'
                                }
                            }
                            MenuItem {
                                text: '30 Seconds'
                                onClicked: {
                                    const alarm = AlarmService.newAlarm()
                                    alarm.time = new Date(new Date().getTime() + 30000)
                                    alarm.name = '30 Seconds from now'
                                }
                            }
                            MenuItem {
                                text: '10 Seconds'
                                onClicked: {
                                    const alarm = AlarmService.newAlarm()
                                    alarm.time = new Date(new Date().getTime() + 10000)
                                    alarm.name = '30 Seconds from now'
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
