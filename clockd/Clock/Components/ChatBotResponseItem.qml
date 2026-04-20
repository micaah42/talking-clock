import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import Clock
import QChatBot

import "../Controls"
import "../ChatBot"

ColumnLayout {
    id: root

    property PromptBuilder promptBuilder: PromptBuilder
    property ChatBotResponse response: null
    property string prompt: ''

    readonly property bool done: response && response.done && text.length === characterTimer.count
    property string text: ''

    signal textAdded
    signal accepted

    spacing: 8
    onResponseChanged: {
        characterTimer.count = 0
        text = ''
    }

    onDoneChanged: {
        if (done)
            TextToSpeechService.say(text)
    }

    Connections {
        target: response
        function onTextReceived(text) {
            root.text += promptBuilder.sanitizeOutput(text)
            root.textAdded()
        }
    }

    Expandable {
        Layout.fillWidth: true
        previewLabel.text: 'Prompt'
        previewLabel.opacity: Theme.o72

        CLabel {
            Layout.fillWidth: true
            size: CLabel.Small
            wrapMode: Text.Wrap
            text: root.prompt
            opacity: Theme.o72
        }
    }

    CLabel {
        Layout.fillWidth: true
        opacity: Theme.o72
        text: 'Response'
    }

    TextEdit {
        id: responseOutput
        Layout.fillWidth: true
        text: root.text.slice(0, characterTimer.count)

        textFormat: Text.PlainText
        color: Material.foreground
        cursorPosition: text.length
        font.pixelSize: Theme.fontSizeMedium
        font.family: FontService.family
        wrapMode: Text.Wrap
        enabled: false

        CLabel {
            visible: !root.done
            x: responseOutput.cursorRectangle.x + 8
            y: responseOutput.cursorRectangle.y - 2

            text: '...'.slice(0, dotsTimer.count % 3 + 1)

            Timer {
                id: dotsTimer
                property int count: 0
                onTriggered: count++
                running: !root.done
                interval: 750
                repeat: true
            }
        }
    }

    RowLayout {
        Layout.alignment: Qt.AlignRight
        spacing: 8

        CToolButton {
            onClicked: root.accepted()
            enabled: root.done
            text: Icons.speaker
        }
        CToolButton {
            onClicked: root.accepted()
            enabled: root.done
            text: Icons.check
        }
    }

    Timer {
        id: characterTimer
        property int count: 0
        running: characterTimer.count < root.text.length
        onTriggered: count += 1
        interval: 15
        repeat: true
    }
}
