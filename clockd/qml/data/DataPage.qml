import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls
import QtMultimedia

import ".."
import Clock 1.0
import Controls 1.0

Item {
    ListView {
        anchors.fill: parent
        model: DataService.datasets
        delegate: Card {
            bright: true
            GridLayout {
                CLabel {
                    text: model.name
                }
                CLabel {
                    text: model.type
                }
                CLabel {
                    text: model.fetches
                }
                CLabel {
                    text: model.bytes
                }
            }
        }
    }

    MediaPlayer {
        id: player
        source: 'qrc:/sounds/cow.mp3'
        loops: MediaPlayer.Infinite

        onErrorOccurred: function (error, errorString) {
            console.warn('player:', error, errorString)
        }

        audioOutput: AudioOutput {//SequentialAnimation on volume {

            //    onFinished: player.stop()
            //    running: true

            //    PropertyAnimation {
            //        duration: 10 * 1000
            //        from: 0.1
            //        to: 1
            //    }

            //    PauseAnimation {
            //        duration: 8 * 1000
            //    }

            //    PropertyAnimation {
            //        duration: 5 * 1000
            //        from: 1
            //        to: 0
            //    }
            //}
        }
    }

    Button {
        text: player.playbackState === MediaPlayer.PlayingState ? 'STOP' : 'PLAY'
        onClicked: player.playbackState === MediaPlayer.PlayingState ? player.stop() : player.play()
    }
}
