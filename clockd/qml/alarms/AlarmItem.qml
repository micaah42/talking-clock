import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

import Clock 1.0
import Controls 1.0

ItemDelegate {
    id: ctrl

    property var alarm: model
    height: 64

    Rectangle {
        anchors.fill: parent
        opacity: 0
        border.color: ColorService.primary
        radius: 5
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 10

        Switch {
            text: checked ? 'Active' : 'Disabled'
            Layout.preferredWidth: 120
            Layout.fillHeight: true
            checked: alarm.activated
            onClicked: alarm.activated = checked
            font.family: FontService.family
            font.pixelSize: 18
        }

        CLabel {
            Layout.fillWidth: true
            text: alarm.name
            font.pixelSize: 18
        }

        CLabel {
            text: new Date(alarm.time).toLocaleTimeString(Qt.locale())
            font.pixelSize: 18
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
                    width: 25
                    scale: 1.15
                    checked: weekdays.getRepeat(modelData)
                    onClicked: weekdays.setRepeat(modelData, checked)
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
