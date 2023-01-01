import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import "qrc:/controls"

ItemDelegate {
    id: ctrl
    width: parent.width
    height: column.height

    property bool expanded: false
    onClicked: expanded = !expanded
    hoverEnabled: !expanded

    Rectangle {
        anchors.fill: parent
        opacity: 0
        border.color: colorService.colors["primary"]
        radius: 5
    }

    ColumnLayout {
        id: column
        width: parent.width

        RowLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            spacing: 10

            Switch {
                text: checked ? 'Active' : 'Disabled'
                Layout.fillHeight: true
                checked: model.activated
                onClicked: model.activated = checked;
                font.family: fontService.family
                font.pixelSize: 18
            }
            CLabel {
                Layout.fillWidth: true
                text: model.name
                font.pixelSize: 18
            }
            CLabel {
                text: new Date(model.time).toLocaleTimeString(Qt.locale())
                font.pixelSize: 18
            }
            Row {
                id: weekdays
                function getRepeat(i) { return  model.repeatRule[i] }
                function setRepeat(i, v) {
                    var rule = model.repeatRule;
                    rule[i] = v;
                    model.repeatRule = rule;
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
        Item {
            Layout.fillWidth: true
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.bottomMargin: 10
            Layout.preferredHeight: childrenRect.height
            clip: true

            ColumnLayout {
                visible: ctrl.height !== 0
                height: ctrl.expanded ? 325 : 0
                width: parent.width
                Behavior on height {
                    PropertyAnimation { duration: 300 }
                }

                CTextField {
                    Layout.fillWidth: true
                    label: 'Name'
                    text: model.name
                    onTextEdited: model.name = text;

                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    TimeField {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        time: model.time
                        onTimeEdited: model.time = time;
                    }

                    Frame {
                        id: sidebar
                        function getSound() { return model.sound; }
                        function setSound(sound) { model.sound = sound; }

                        Layout.preferredWidth: parent.width / 3
                        Layout.fillHeight: true

                        ColumnLayout {
                            anchors.margins: 5
                            anchors.fill: parent
                            CTextField {
                                Layout.fillWidth: true
                                label: "Program"
                            }
                            CTextField {
                                Layout.fillWidth: true
                                label: "Arguments"
                            }
                            CComboBox {
                                Layout.fillWidth: true
                                label: "Sound"
                                model: soundService.availableSounds;
                                text: sidebar.getSound()
                                onActivated: {
                                    sidebar.setSound(soundService.availableSounds[index])
                                }

                            }

                            Button {
                                Layout.alignment: Qt.AlignRight
                                highlighted: true
                                text: "Remove"
                                onClicked: alarms.model.remove(index);
                            }
                        }
                    }
                }
            }
        }
    }
}
