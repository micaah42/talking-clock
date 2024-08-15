import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQuick.Shapes 1.15

import Clock 1.0
import Clock.Controls 1.0
import Clock.Pages.AlarmPage 1.0

Item {
    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            Layout.maximumWidth: 0.3 * parent.width
            Layout.fillHeight: true
            Layout.margins: 8
            spacing: 8

            Component {
                id: nextAlarmComponent
                NextAlarm {
                    anchors.fill: parent
                    alarm: AlarmService.nextAlarm
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
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.maximumHeight: 0.5 * parent.height
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

                            x: 150 * (position * position) + 105
                            y: -100 * position - 8

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
                    width: view.width
                    alarm: model.value
                }

                header: Button {
                    width: parent.width
                    Material.roundedScale: Material.ExtraSmallScale
                    font.family: FontService.family
                    text: "New Alarm"
                    onClicked: {
                        AlarmService.newAlarm()
                    }
                }
            }
        }
    }
}
