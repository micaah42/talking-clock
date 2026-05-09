import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Clock
import "../Style"
import "../Controls"

ColumnLayout {
    id: root
    property Window window: Window.window
    property Alarm alarm
    spacing: 8

    Component {
        id: nextAlarmComponent

        Item {
            implicitHeight: nextAlarmColumn.implicitHeight

            CToolButton {
                anchors.right: parent.right
                anchors.top: parent.top
                onClicked: editDialog.open()
                text: Icons.edit
                flat: false
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
                    visible: alarm.name
                    labelText: "Name"
                    valueText: alarm.name
                }

                ColumnLayout {
                    spacing: -2

                    ValueDisplay {
                        labelText: "Time"
                        valueText: alarm.nextTimeout.toLocaleTimeString(Qt.locale(), Locale.ShortFormat)
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
                    valueText: capitalize(SoundService.displayName(alarm.sound))
                    visible: alarm.sound

                    function capitalize(string) {
                        return string.charAt(0).toUpperCase() + string.slice(1)
                    }
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

    AlarmDialog {
        id: editDialog
        parent: root.window.contentItem
        alarm: root.alarm
    }
}
