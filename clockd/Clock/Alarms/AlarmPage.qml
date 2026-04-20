import QtQuick
import QtQuick.Layouts 1.14
import QtQuick.Controls
import QtQuick.Shapes 1.15

import Clock
import "../Style"
import "../Controls"

Item {
    RowLayout {
        anchors.fill: parent

        CFrame {
            Layout.preferredWidth: parent.width / 3
            Layout.fillHeight: true
            Layout.margins: 8
            clip: true

            NextAlarm {
                alarm: AlarmService.nextAlarm
                width: parent.width
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                id: view

                anchors.fill: parent
                spacing: 16
                clip: true

                model: SortFilterAlarmModel {
                    listModel: AlarmService.model
                }

                header: RowLayout {
                    width: parent.width
                    height: 56

                    CButton {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        text: "New Alarm"
                        onClicked: {
                            const alarm = AlarmService.newAlarm()
                            alarm.sound = SoundService.availableSounds[0] || ''
                            AlarmService.addAlarm(alarm)
                            alarmDialog.alarm = alarm
                            alarmDialog.open()
                        }
                    }

                    CButton {
                        Layout.preferredWidth: height
                        Layout.fillHeight: true
                        font.family: Icons.fontFamily
                        text: Icons.more_vert
                        onClicked: menu.open()

                        Menu {
                            id: menu
                            x: parent.width - width
                            y: parent.height + 8

                            CMenuItem {
                                text: 'New Timer'
                                onClicked: {
                                    const alarm = AlarmService.newAlarm()
                                    alarm.singleShot = true
                                    timerDialog.alarm = alarm
                                    timerDialog.open()
                                }
                            }

                            CMenuItem {
                                text: 'Remove all'
                                onClicked: removeAllDialog.open()

                                Dialog {
                                    id: removeAllDialog
                                    title: 'Remove all alarms?'
                                    standardButtons: Dialog.Ok | Dialog.Cancel
                                    onAccepted: AlarmService.removeAllAlarms()
                                    anchors.centerIn: Overlay.overlay
                                }
                            }
                        }
                    }
                }

                delegate: AlarmItem {
                    width: view.width
                    alarm: modelData

                    onClicked: {
                        if (wasHeld) {
                            wasHeld = false
                            return
                        }

                        alarmDialog.alarm = alarm
                        alarmDialog.open()
                    }
                }
            }
        }
    }

    Dialog {
        id: timerDialog
        property Alarm alarm

        anchors.centerIn: parent
        standardButtons: Dialog.Ok | Dialog.Cancel
        Material.roundedScale: Material.ExtraSmallScale
        closePolicy: Popup.NoAutoClose
        title: 'Create Timer'

        contentItem: DurationEdit {
            id: durationEdit
        }

        onRejected: alarm.destroy()
        onAccepted: {
            alarm.name = `Timer (${Theme.durationString(durationEdit.msecs, Locale.NarrowFormat)})`
            alarm.time = new Date(AlarmService.now.getTime() + durationEdit.msecs)
            AlarmService.addAlarm(alarm)
        }
    }
    AlarmDialog {
        id: alarmDialog
    }

    Dialog {
        id: eventDialog
    }
}
