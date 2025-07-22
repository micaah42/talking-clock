import QtQuick 
import QtQuick.Layouts 1.14
import QtQuick.Controls 
import QtQuick.Controls.Material
import QtQuick.Shapes 1.14

import Clock 1.0
import Clock.Style
import Clock.Controls 1.0

ColumnLayout {
    id: root
    property Alarm alarm
    spacing: 8

    Component {
        id: nextAlarmComponent

        Item {
            implicitHeight: nextAlarmColumn.implicitHeight

            Icon {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: -implicitHeight / 4
                font.pixelSize: parent.width
                opacity: Theme.o11
                text: {
                    const hours = alarm.nextTimeout.getHours()
                    if (8 <= hours && hours < 22)
                        return Icons.sunny
                    else
                        return Icons.clear_night
                }
            }

            Rectangle {
                anchors.right: parent.right
                anchors.top: parent.top
                color: Theme.accent

                radius: width / 2
                height: 48
                width: 48

                CToolButton {
                    anchors.centerIn: parent
                    onClicked: editDialog.active = true
                    text: Icons.edit
                }
            }

            ColumnLayout {
                id: nextAlarmColumn
                width: parent.width
                spacing: 8

                CLabel {
                    size: CLabel.XLarge
                    text: 'Next Alarm:'
                }

                ValueDisplay {
                    visible: alarm.name.length
                    labelText: "Name"
                    valueText: alarm.name
                }

                ColumnLayout {
                    spacing: -2

                    ValueDisplay {
                        labelText: "Time"
                        valueText: alarm.nextTimeout.toLocaleTimeString()
                    }

                    CLabel {
                        text: {
                            const msecs = alarm.nextTimeout.getTime() - AlarmService.now.getTime()
                            return `in ${Theme.durationString(msecs)}`
                        }
                    }
                }

                ValueDisplay {
                    labelText: "Sound"
                    valueText: alarm.sound === '' ? 'No Sound!' : capitalize(SoundService.displayName(alarm.sound))

                    function capitalize(string) {
                        return string.charAt(0).toUpperCase() + string.slice(1)
                    }
                }
            }

            Loader {
                id: editDialog
                asynchronous: true
                active: false
                sourceComponent: AlarmDialog {
                    visible: true
                    onAboutToHide: editDialog.active = false
                    alarm: root.alarm
                }
            }
        }
    }

    Component {
        id: noNextAlarmComponent

        RowLayout {
            ValueDisplay {
                Layout.alignment: Qt.AlignTop
                labelText: 'No Alarms scheduled : )'
                valueText: 'Sleep tight!'
            }
            SleepyBed {
                Layout.alignment: Qt.AlignRight
            }
        }
    }

    Loader {
        id: nextAlarmLoader
        Layout.fillWidth: true
        sourceComponent: alarm ? nextAlarmComponent : noNextAlarmComponent
    }
}
