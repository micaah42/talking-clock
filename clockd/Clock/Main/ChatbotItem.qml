import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock
import QChatBot

import "../Style"
import "../Controls"
import "../Components"

ColumnLayout {
    id: root

    property real animationOpacity: Theme.o24

    readonly property bool animating: runningResponses.length
    property var runningResponses: []

    RowLayout {
        Layout.fillWidth: true

        CLabel {
            Layout.fillWidth: true
            text: `Clock is chiming in...`
            size: CLabel.XLarge
            Layout.bottomMargin: 4
        }
        Item {
            Layout.alignment: Qt.AlignTop

            Icon {
                anchors.right: parent.right
                font.pixelSize: Theme.fontSizeXXLarge
                text: Icons.robot_2

                Icon {
                    anchors.margins: -24
                    anchors.leftMargin: -8
                    anchors.bottom: parent.top
                    anchors.left: parent.right
                    text: Icons.lightbulb
                    opacity: root.animating

                    Behavior on opacity {
                        PropertyAnimation {}
                    }
                }

                SequentialAnimation on opacity {
                    loops: Animation.Infinite
                    running: root.animating
                    alwaysRunToEnd: true

                    OpacityAnimator {
                        duration: 1000
                        from: 1
                        to: animationOpacity
                    }
                    OpacityAnimator {
                        duration: 1000
                        from: animationOpacity
                        to: 1
                    }
                }
            }
        }
    }

    Repeater {
        model: ChatBotInit.listModel
        delegate: ChatBotResponseItem {
            id: responseItem

            Component.onDestruction: runningResponses = runningResponses.filter(x => x !== responseItem.response)
            Component.onCompleted: runningResponses = runningResponses.concat([response])
            onDoneChanged: {
                if (responseItem.done) {
                    runningResponses = runningResponses.filter(x => x !== responseItem.response)
                }
            }

            onAccepted: ChatBotInit.listModel.removeAt(index, 1)
            Layout.fillWidth: true
            response: modelData
        }
    }

    CButton {
        onClicked: ChatBotInit.addBasicPrompt()
        font.family: Icons.fontFamily
        text: Icons.chat
    }
}
