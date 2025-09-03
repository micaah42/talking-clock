import QtQuick 
import QtQuick.Layouts 1.14
import QtQuick.Controls 

import Clock
import "../Controls"
import "../Style"

ItemDelegate {
    id: ctrl

    property Alarm alarm

    property bool wasHeld: false

    onPressAndHold: wasHeld = true

    Rectangle {
        anchors.fill: parent
        border.color: Theme.primary
        opacity: 0
        radius: 5
    }

    contentItem: Loader {
        width: parent.width
        active: alarm !== null

        sourceComponent: RowLayout {
            id: row
            spacing: 16

            Switch {
                Layout.preferredWidth: 64
                Layout.fillHeight: true
                checked: alarm.activated
                onClicked: alarm.activated = checked
                font.family: FontService.family
            }

            CLabel {
                Layout.fillWidth: true
                text: alarm.name
                size: CLabel.Large
            }

            CLabel {
                text: new Date(alarm.time).toLocaleTimeString(Qt.locale())
                size: CLabel.Large
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
                    delegate: CheckBox {
                        onClicked: weekdays.setRepeat(modelData, checked)
                        checked: weekdays.getRepeat(modelData)
                        scale: 1.25
                        width: 27
                    }
                }
                Item {
                    height: parent.parent.height
                    width: 0.1
                    Button {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        height: row.height
                        highlighted: true

                        width: wasHeld ? weekdays.width : 0
                        Behavior on width {
                            PropertyAnimation {}
                        }

                        onClicked: AlarmService.removeAlarm(alarm)
                        font.family: Icons.fontFamily
                        text: Icons._delete
                        visible: wasHeld
                    }
                }
            }
        }
    }
}
