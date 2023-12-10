import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    width: 1000
    height: 1000 * 9 / 16
    visible: true
    ColumnLayout {
        anchors.fill: parent

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            model: cModel.model
            delegate: ItemDelegate {
                id: del
                property int integer
                property string str
                Row {
                    TextField {
                        text: model.integer
                        onTextEdited: model.integer = text
                        validator: IntValidator {}
                    }
                    TextField {
                        text: model.str
                        onTextEdited: { model.str = text; }
                    }
                }
                Component.onCompleted: {
                    console.log('complete!')
                }
            }
        }
        Button {
            onClicked: {
                cModel.model.append();
            }
        }
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            model: cModel.model
            delegate: ItemDelegate {
                property int integer
                property string str
                Row {
                    TextField {
                        text: model.integer
                        onTextEdited: model.integer = text
                        validator: IntValidator {}
                    }
                    TextField {
                        text: model.str
                        onTextEdited: { model.str = text; }
                    }
                }
            }
        }
    }
}
