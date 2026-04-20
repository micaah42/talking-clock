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

    property LLamaCppService ollamaService: LLamaCppService
    property PromptBuilder promptBuilder: PromptBuilder

    function addPrompt(prompt) {
        const element = {
            "prompt": prompt
        }

        listModel.append(element)
    }

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
        model: ListModel {
            id: listModel
        }

        delegate: ChatBotResponseItem {
            id: responseItem

            Component.onDestruction: runningResponses = runningResponses.filter(x => x !== responseItem.response)
            Component.onCompleted: runningResponses = runningResponses.concat([response])
            onDoneChanged: {
                if (responseItem.done) {
                    console.log(runningResponses, responseItem.response)
                    runningResponses = runningResponses.filter(x => x !== responseItem.response)
                }
            }

            Layout.fillWidth: true
            response: ollamaService.generate(model.prompt, '', responseItem)
            onAccepted: listModel.remove(index, 1)
            prompt: model.prompt
        }
    }

    function addBasicPrompt(extraString) {
        const alarms = AlarmService.nextAlarm ? [AlarmService.nextAlarm] : []
        const prompt = promptBuilder.create(alarms, [], ActionDayService.days, PromptBuilderSettings.mood,
                                            extraString || '')
        addPrompt(prompt)
    }

    CButton {
        onClicked: addBasicPrompt()
        font.family: Icons.fontFamily
        text: Icons.chat
    }

    Connections {
        target: AlarmService

        function onAlarmTriggered(_alarm) {
            console.log('shoopw', _alarm)
            timer.alarm = _alarm
            timer.start()
        }
    }

    Timer {
        id: timer
        property Alarm alarm

        interval: 10 * 1000
        onTriggered: {
            console.log('shoopw22', timer.alarm)
            if (!timer.alarm)
                return

            const prompt = promptBuilder.create([], [timer.alarm], ActionDayService.days, PromptBuilderSettings.mood)
            addPrompt(prompt)
        }
    }

    Connections {

        target: TimeService

        function onNowHoursChanged() {
            if (TimeService.now.getHours() === 12) {
                addBasicPrompt('It\'s 12 o clock now!')
            }
        }

        function onNowMinutesChanged() {
            if (TimeService.now.getHours() === 21 && TimeService.now.getMintutes() === 12) {
                addBasicPrompt('It\'s 2112 now, like in the rush song!')
            }
        }
    }
}
