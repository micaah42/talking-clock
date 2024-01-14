import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQuick.Shapes 1.15

import "."
import ".."
import Clock 1.0
import Controls 1.0

Item {
    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 16

        Frame {
            Layout.alignment: Qt.AlignTop
            Layout.preferredWidth: 3 * parent.width / 7
            Layout.fillHeight: true
            padding: 16

            ColumnLayout {
                anchors.fill: parent

                Loader {
                    id: nextAlarmLoader

                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: alarm
                    active: alarm

                    property var alarm: {
                        if (AlarmService.nextIds.length === 0)
                            return null
                        if (AlarmService.model.size() <= AlarmService.nextIds[0])
                            return null
                        return AlarmService.model.at(AlarmService.nextIds[0])
                    }

                    sourceComponent: NextAlarm {
                        anchors.fill: parent
                        alarm: parent.alarm
                    }
                }

                CLabel {
                    visible: !nextAlarmLoader.active
                    font.pixelSize: 42
                    text: 'No Alarms scheduled : )'
                }
                CLabel {
                    visible: !nextAlarmLoader.active
                    font.pixelSize: 32
                    text: 'Sleep tight!'
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
                        model: 5
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

                                x: 96 * (position * position) + 172
                                y: -96 * position + 16

                                opacity: {
                                    if (position < 0.2)
                                        return 5 * position
                                    if (position > 0.8)
                                        return 5 - (5 * position)

                                    return 1
                                }

                                font.pixelSize: 22 + 42 * position * position
                                rotation: 15 * Math.random()
                                text: 'z'
                            }
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: view
                anchors.margins: 5
                anchors.fill: parent
                spacing: 5
                clip: true

                model: AlarmService.model
                delegate: AlarmItem {
                    width: view.width
                }

                footer: Button {
                    width: parent.width
                    Material.roundedScale: Material.ExtraSmallScale
                    font.family: FontService.family
                    text: "New Alarm"
                    onClicked: {
                        AlarmService.model.push()
                    }
                }
            }
        }
    }
}
