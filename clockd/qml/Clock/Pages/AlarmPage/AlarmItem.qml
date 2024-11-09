import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

import Clock 1.0
import Clock.Controls 1.0

ItemDelegate {
    id: ctrl

    property Alarm alarm
    height: 64

    Rectangle {
        anchors.fill: parent
        border.color: ColorService.primary
        opacity: 0
        radius: 5
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 16

        Switch {
            // text: checked ? 'Active' : 'Disabled'
            Layout.preferredWidth: 64
            Layout.fillHeight: true
            checked: alarm.activated
            onClicked: alarm.activated = checked
            font.family: FontService.family
            font.pixelSize: 18
        }

        CLabel {
            Layout.fillWidth: true
            text: alarm.name
            font.pixelSize: 24
        }

        CLabel {
            text: new Date(alarm.time).toLocaleTimeString(Qt.locale())
            font.pixelSize: 22
        }

        Row {
            id: weekdays
            Layout.bottomMargin: 4
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
        }
    }

    onClicked: {
        editDialog.active = true
    }

    Loader {
        id: editDialog
        asynchronous: true
        active: false

        sourceComponent: AlarmDialog {
            alarm: ctrl.alarm
            onAboutToHide: editDialog.active = false
        }
    }
}
