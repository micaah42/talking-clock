import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import Clock 1.0
import Clock.Style 1.0
import Clock.Controls 1.0

Popup {
    property Alarm alarm

    Material.roundedScale: Material.ExtraSmallScale
    visible: true

    closePolicy: Popup.NoAutoClose
    parent: window.contentItem
    y: (window.height - height) / 2
    x: (window.width - width) / 2
    height: 0.8 * window.height
    width: 0.8 * window.width
    dim: true

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
                Layout.bottomMargin: 6
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
                    delegate: CheckBox {
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
            labelText: 'Name'
            text: alarm.name
            onTextEdited: alarm.name = text
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            TimeField {
                id: timeField
                Layout.fillHeight: true
                Layout.fillWidth: true
                time: alarm.time
                onTimeChanged: alarm.time = time
            }

            ColumnLayout {
                Layout.minimumWidth: parent.width / 3
                Layout.maximumWidth: parent.width / 3
                Layout.fillHeight: true

                ListView {
                    id: list
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    currentIndex: model.indexOf(alarm.sound)
                    model: SoundService.availableSounds
                    clip: true

                    header: CLabel {
                        text: 'Sound'
                        padding: 4
                    }

                    delegate: ItemDelegate {
                        id: d
                        Material.background: Material.Red
                        highlighted: ListView.isCurrentItem
                        onClicked: alarm.sound = modelData
                        width: list.width
                        RowLayout {
                            anchors.fill: parent
                            anchors.rightMargin: 16
                            anchors.leftMargin: 16
                            CLabel {
                                Material.foreground: d.highlighted ? Material.Red : parent.Material.foreground
                                font.pixelSize: 18
                                text: modelData.split('.')[0]
                            }
                            Item {
                                Layout.fillWidth: true
                            }
                            ToolButton {
                                font.family: Icons.fontFamily
                                Material.foreground: d.highlighted ? Material.Red : parent.Material.foreground
                                text: Icons.play_circle
                                font.pixelSize: 18
                            }
                        }
                    }
                }

                DelayButton {
                    Layout.bottomMargin: -8
                    Layout.fillWidth: true

                    Material.background: ColorService.accent
                    property bool wasActivated: false
                    font.pixelSize: 18
                    text: "Remove"

                    onActivated: wasActivated = true
                    delay: 1000

                    onReleased: {
                        if (wasActivated) {
                            AlarmService.model.removeAt(index)
                            close()
                        }
                    }
                }

                Button {
                    Material.background: ColorService.primary
                    Layout.bottomMargin: -4
                    Layout.fillWidth: true
                    onClicked: close()
                    text: 'OK'
                }
            }
        }
    }
}
