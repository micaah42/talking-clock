import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import Controls 1.0

import Clock 1.0

Dialog {
    property var alarm

    Material.roundedScale: Material.ExtraSmallScale
    visible: true

    parent: window.contentItem
    scale: window.s === 0 ? 1 : 1 / window.s
    y: (window.h - height) / 2
    x: (window.w - width) / 2
    height: 0.8 * window.h
    width: 0.8 * window.w
    dim: false

    ColumnLayout {
        id: column
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            spacing: 10

            Switch {
                text: checked ? 'Active' : 'Disabled'
                Layout.fillHeight: true
                checked: alarm.activated
                onClicked: alarm.activated = checked
                font.family: FontService.family
                font.pixelSize: 18
                scale: 1.4
            }

            Item {
                Layout.fillWidth: true
            }

            Row {
                id: weekdays

                function getRepeat(i) {
                    return alarm.repeatRule[i]
                }
                function setRepeat(i, v) {
                    var rule = alarm.repeatRule
                    rule[i] = v
                    alarm.repeatRule = rule
                }
                Repeater {
                    model: 7
                    delegate: CCheckBox {
                        width: 32
                        scale: 1.5
                        checked: weekdays.getRepeat(modelData)
                        onClicked: weekdays.setRepeat(modelData, checked)
                    }
                }
            }
        }

        CTextField {
            Layout.fillWidth: true
            label: 'Name'
            text: alarm.name
            onTextEdited: alarm.name = text
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            TimeField {
                Layout.fillWidth: true
                Layout.fillHeight: true
                time: alarm.time
                onTimeEdited: alarm.time = time
            }

            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Frame {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    CComboBox {
                        onActivated: {
                            alarm.sound = SoundService.availableSounds[currentIndex]
                        }
                        model: SoundService.availableSounds
                        text: alarm.sound
                        label: "Sound"
                        width: parent.width
                    }
                }

                Frame {
                    Layout.fillWidth: true

                    DelayButton {
                        width: parent.width
                        property bool wasActivated: false
                        text: "Remove"
                        delay: 2000
                        onActivated: wasActivated = true
                        onReleased: {
                            if (wasActivated) {
                                AlarmService.model.remove(index)
                                accept()
                            }
                        }
                    }
                }
            }
        }

        RowLayout {
            Item {
                Layout.fillWidth: true
            }
        }
    }
}
