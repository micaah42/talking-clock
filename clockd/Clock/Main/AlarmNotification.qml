import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtMultimedia

import Clock
import "../Controls"
import "../Style"

ColumnLayout {
    id: root
    property Alarm alarm
    property bool accepted: false
    spacing: 8

    function muteAlarm() {
        player.stop()
    }

    MediaPlayer {
        id: player

        Component.onCompleted: play()
        loops: 16

        source: {
            const sound = alarm.sound ? alarm.sound : SoundService.availableSounds[0] || ''
            if (sound === '')
                return ''

            return `file://${sound}`
        }

        onErrorOccurred: function (error, errorString) {
            console.warn(error, errorString)
        }

        audioOutput: AudioOutput {
            SequentialAnimation on volume {
                onFinished: player.stop()
                running: true
                loops: 2

                PropertyAnimation {
                    duration: 8 * 1000
                    from: 0
                    to: SoundService.volume
                }

                PauseAnimation {
                    duration: 16 * 1000
                }

                PropertyAnimation {
                    duration: 8 * 1000
                    from: SoundService.volume
                    to: 0
                }
            }
        }
    }

    RowLayout {
        spacing: 16
        ValueDisplay {
            id: valueDisplay
            Layout.preferredWidth: parent.width / 2

            property bool isUntitled: alarm.name === ''
            value.opacity: isUntitled ? Theme.o56 : 1
            valueText: isUntitled ? '(Untitled Alarm)' : alarm.name
            labelText: 'Name'
        }
        Rectangle {
            Layout.preferredHeight: valueDisplay.implicitHeight
            visible: alarm.name !== ''
            Layout.preferredWidth: 1
            Layout.rightMargin: 4
            Layout.leftMargin: 4
            opacity: Theme.o24
        }
        ValueDisplay {
            Layout.preferredWidth: parent.width / 2
            valueText: alarm.time.toLocaleTimeString(Qt.locale(), Locale.ShortFormat)
            labelText: 'Time'
        }
    }
}
