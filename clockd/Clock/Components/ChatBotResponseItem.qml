import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtMultimedia

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

    spacing: 0
    onResponseChanged: {
        text = response?.text || ''
        characterTimer.count = 0
    }

    Component.onCompleted: {
        text = response?.text || ''
    }

    Connections {
        target: response
        function onTextReceived(text) {
            root.text += promptBuilder.sanitizeOutput(text)
            root.textAdded()
        }
        function onTtsStatusChanged() {
            if (response.ttsStatus === ChatBotResponse.Done) {
                player.source = `file://${response.ttsOutputFile}`
            }
        }
    }

    MediaPlayer {
        id: player
        audioOutput: AudioOutput {}
    }

    FlickableColumnLayout {
        id: flickable

        Layout.fillHeight: true
        Layout.fillWidth: true
        clip: true

        TextEdit {
            id: responseOutput

            Layout.fillWidth: true
            text: root.text.slice(0, characterTimer.count)
            width: flickable.width

            textFormat: Text.PlainText
            color: Material.foreground
            cursorPosition: text.length
            font.pixelSize: Theme.fontSizeMedium
            font.family: FontService.family
            wrapMode: Text.Wrap
            opacity: Theme.o84
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
    }

    Slider {
        Layout.topMargin: -8
        Layout.bottomMargin: -24
        Layout.fillWidth: true
        onValueChanged: player.position = value
        value: player.position
        to: player.duration
        live: false
        z: 1
    }

    RowLayout {
        enabled: root.response.ttsStatus === ChatBotResponse.Done
        Layout.alignment: Qt.AlignHCenter
        spacing: 8

        CToolButton {
            onClicked: player.position -= 5000
            text: Icons.fast_rewind
        }
        CToolButton {
            onClicked: player.playing ? player.stop() : player.play()
            text: player.playing ? Icons.stop : Icons.play_arrow
        }
        CToolButton {
            onClicked: player.position += 5000
            text: Icons.fast_forward
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

    Timer {
        id: autoAccept
        onTriggered: root.accepted()
        interval: 600 * 1000
        running: true
    }
}
