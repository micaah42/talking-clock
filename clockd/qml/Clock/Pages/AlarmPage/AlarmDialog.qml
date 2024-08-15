import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import Clock 1.0
import Clock.Controls 1.0

Dialog {
    property Alarm alarm

    Material.roundedScale: Material.ExtraSmallScale
    visible: true

    parent: window.contentItem
    scale: window.s === 0 ? 1 : 1 / window.s
    y: (window.height - height) / 2
    x: (window.width - width) / 2
    height: 0.8 * window.height
    width: 0.8 * window.width
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
                id: timeField
                Layout.fillWidth: true
                Layout.fillHeight: true
                time: alarm.time
                onTimeChanged: alarm.time = time
            }

            ColumnLayout {
                Layout.maximumWidth: parent.width / 3
                Layout.fillHeight: true

                CComboBox {
                    Layout.fillWidth: true
                    onActivated: alarm.sound = SoundService.availableSounds[currentIndex]
                    model: SoundService.availableSounds
                    text: alarm.sound
                    label: "Sound"
                }

                Item {
                    Layout.fillHeight: true
                }

                DelayButton {
                    Layout.bottomMargin: -6
                    Layout.fillWidth: true

                    property bool wasActivated: false
                    text: "Remove"

                    onActivated: wasActivated = true
                    delay: 2000

                    onReleased: {
                        if (wasActivated) {
                            AlarmService.model.removeAt(index)
                            accept()
                        }
                    }
                }
            }
        }
    }
}
