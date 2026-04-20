import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import Clock
import QChatBot

import "../Style"
import "../Controls"
import "../Components"

Section {
    id: root
    title: 'ChatBot'
    icon: Icons.robot_2

    property LLamaCppService _ollamaService: LLamaCppService
    property PromptBuilder _promptBuilder: PromptBuilder

    RowLayout {
        Layout.fillWidth: true

        CComboBox {
            id: models

            Layout.fillWidth: true
            placeholderText: 'Model'
            model: _ollamaService.models
            currentIndex: 1
        }

        CComboBox {
            id: moods
            Layout.fillWidth: true
            placeholderText: 'Mood'
            readonly property var values: [//@
                PromptBuilder.Enthusiastic, //@
                PromptBuilder.Knightly, //@
                PromptBuilder.Angry, //@
                PromptBuilder.Deranged, //@
                PromptBuilder.Stoned, //@
                PromptBuilder.Depressed, //@
                PromptBuilder.Alien, //@
                PromptBuilder.SlightlyUpset, //@
                PromptBuilder.HomicidalSpaceAI, //@
                PromptBuilder.Formal //@
            ]
            model: [//@
                'Enthusiastic', //@
                'Knightly', //@
                'Angry', //@
                'Deranged', //@
                'Stoned', //@
                'Depressed', //@
                'Alien', //@
                'SlightlyUpset', //@
                'HomicidalSpaceAI', //@
                'Formal' //@
            ]

            currentIndex: values.indexOf(PromptBuilderSettings.mood)
            onActivated: function (index) {
                PromptBuilderSettings.mood = values[index]
            }
        }
        CButton {
            Layout.alignment: Qt.AlignBottom

            text: Icons.send
            font.pixelSize: Theme.fontSizeLarge
            font.family: Icons.fontFamily

            onClicked: {
                const alarms = AlarmService.nextAlarm ? [AlarmService.nextAlarm] : []
                const prompt = _promptBuilder.create(alarms, ActionDayService.days, [],
                                                     moods.values[moods.currentIndex])
                const modelId = models.model[models.currentIndex]
                const response = _ollamaService.generate(prompt, modelId, root)

                const object = {
                    "response": response,
                    "prompt": prompt,
                    "modelId": modelId
                }

                listModel.append(object)
            }
        }
    }
    ListView {
        id: list

        Layout.fillHeight: true
        Layout.fillWidth: true
        spacing: 0
        clip: true

        model: ListModel {
            id: listModel
        }

        delegate: ColumnLayout {
            id: d
            property ChatBotResponse response: model.response
            property string modelId: model.modelId
            property string prompt: model.prompt

            width: list.width

            CLabel {
                Layout.bottomMargin: 8
                Layout.fillWidth: true
                wrapMode: Text.Wrap
                size: CLabel.Large
                text: 'Prompt:'
            }
            Item {
                Layout.preferredHeight: promptEdit.implicitHeight
                Layout.fillWidth: true

                TextEdit {
                    id: promptEdit
                    width: parent.width
                    textFormat: Text.PlainText
                    color: Material.foreground
                    cursorPosition: text.length
                    font.pixelSize: Theme.fontSizeMedium
                    font.family: FontService.family
                    wrapMode: Text.Wrap
                    enabled: false

                    text: d.prompt
                }

                CToolButton {
                    anchors.right: parent.right
                    text: Icons.copy_all

                    onClicked: {
                        promptEdit.selectAll()
                        promptEdit.copy()
                        promptEdit.select(0, 0)
                    }
                }
            }
            CLabel {
                Layout.fillWidth: true
                wrapMode: Text.Wrap
                opacity: Theme.o72
                size: CLabel.Small
                text: d.modelId
            }
            CLabel {
                Layout.bottomMargin: 8
                Layout.fillWidth: true
                wrapMode: Text.Wrap
                size: CLabel.Large
                text: 'Response:'
            }
            ChatBotResponseItem {
                id: responseItem
                Layout.fillWidth: true
                onTextChanged: list.positionViewAtEnd()
                promptBuilder: _promptBuilder
                response: d.response
            }
        }
    }
}
