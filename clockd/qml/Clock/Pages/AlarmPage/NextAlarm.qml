import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQuick.Shapes 1.14

import Clock 1.0
import Clock.Controls 1.0

ColumnLayout {
    property Alarm alarm
    spacing: 8

    Component {
        id: nextAlarmComponent
        Item {

            Image {
                anchors.right: parent.right
                anchors.top: parent.top

                sourceSize.height: 64
                sourceSize.width: 64

                source: {
                    const hours = alarm.nextTimeout.getHours()
                    if (8 <= hours && hours < 22)
                        return 'qrc:/sunny_FILL0_wght400_GRAD0_opsz24.svg'
                    else
                        return 'qrc:/clear_night_FILL0_wght400_GRAD0_opsz24.svg'
                }
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 12

                ValueDisplay {
                    labelText: "Name"
                    valueText: alarm.name
                }

                ColumnLayout {
                    spacing: -4

                    ValueDisplay {
                        labelText: "Time"
                        valueText: alarm.nextTimeout.toLocaleTimeString()
                    }

                   CLabel {
                        font.pixelSize: 20

                        Timer {
                            onTriggered: parent.text = `triggered in ${durationString()}`
                            triggeredOnStart: true
                            running: true
                            repeat: true

                            function durationString() {
                                const msecs = alarm.nextTimeout.getTime() - (new Date()).getTime()
                                const secs = Math.round(msecs / 1000)

                                if (secs < 60) {
                                    return `${secs} seconds`
                                }

                                const minutes = Math.round(secs / 60)

                                if (minutes < 60) {
                                    return `${minutes} minutes`
                                }

                                const hours = Math.round(minutes / 60)

                                if (hours < 24) {
                                    return `${hours} hours`
                                }

                                const days = hours / 24
                                return `${days.toFixed(1)} days`
                            }
                        }
                    }
                }

                ValueDisplay {
                    labelText: "Sound"
                    valueText: alarm.sound === '' ? 'No Sound!' : capitalize(alarm.sound.split('.')[0])

                    function capitalize(string) {
                        return string.charAt(0).toUpperCase() + string.slice(1)
                    }
                }
            }
        }
    }

    Component {
        id: noNextAlarmComponent
        ValueDisplay {
            labelText: 'No Alarms scheduled : )'
            valueText: 'Sleep tight!'
        }
    }

    Loader {
        id: nextAlarmLoader
        Layout.fillWidth: true
        sourceComponent: AlarmService.nextAlarm ? nextAlarmComponent : noNextAlarmComponent
    }

    Item {
        Layout.fillHeight: true
    }

    Image {
        id: bed
        Layout.alignment: Qt.AlignHCenter
        Layout.preferredWidth: 96
        fillMode: Image.PreserveAspectFit
        source: 'qrc:/bed_FILL0_wght400_GRAD0_opsz24.svg'
        sourceSize.width: width
        sourceSize.height: height

        Repeater {
            id: zzz
            model: 3
            delegate: Item {
                CLabel {
                    property real range

                    PropertyAnimation on range {
                        loops: Animation.Infinite
                        duration: 8000
                        from: 0
                        to: 1
                    }

                    property real position: {
                        const p = range + modelData / zzz.count
                        if (p < 1)
                            return p
                        else
                            return p - 1
                    }

                    x: bed.width * (position * position + 0.5) * 0.8
                    y: -bed.height * position * 0.8 - 32

                    opacity: {
                        if (position < 0.2)
                            return 5 * position
                        if (position > 0.8)
                            return 5 - (5 * position)

                        return 1
                    }

                    font.pixelSize: 48 + 48 * position * position
                    rotation: 15 * Math.random()
                    text: 'z'
                }
            }
        }
    }
}
