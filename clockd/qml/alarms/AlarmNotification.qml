import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtMultimedia

import Clock 1.0
import ".."

Item {
    property var alarm
    property var dtor

    width: window.width
    height: window.height / 2
    anchors.bottom: parent.bottom

    MouseArea {
        // avoid events being passed through
        anchors.fill: parent
    }

    MediaPlayer {
        id: player
        Component.onCompleted: {
            console.log('playing', source)
            play()
        }
        source: alarm.sound.length ? `qrc:/sounds/${alarm.sound}` : ''
        loops: MediaPlayer.Infinite

        onErrorOccurred: function (error, errorString) {
            console.warn(error, errorString)
        }

        audioOutput: AudioOutput {
            SequentialAnimation on volume {
                onFinished: player.stop()
                running: true

                PropertyAnimation {
                    duration: 10 * 1000
                    from: 0.1
                    to: 1
                }

                PauseAnimation {
                    duration: 8 * 1000
                }

                PropertyAnimation {
                    duration: 5 * 1000
                    from: 1
                    to: 0
                }
            }
        }
    }

    Card {
        id: bg
        anchors.fill: parent
        anchors.margins: 16
        opacity: 0.9

        Timer {
            onTriggered: bg.bright = !bg.bright
            interval: 500
            running: true
            repeat: true
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 32

        Label {
            text: alarm.name
            font.pixelSize: 72
            leftPadding: 8
        }

        Item {
            Layout.fillHeight: true
        }

        RowLayout {
            id: row
            Layout.preferredHeight: 72
            Layout.fillWidth: true
            spacing: 24

            Slider {
                id: slider
                Layout.preferredHeight: parent.height
                Layout.fillWidth: true
                handle.height: 0.8 * height
                handle.width: 1.66 * height
                background.transform: Scale {
                    yScale: 2
                    origin.y: 2
                }

                // mouse areas to restrict interactivity to handle
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPressed: function (mouse) {
                        mouse.accepted = !handleArea.containsMouse
                    }
                }

                MouseArea {
                    id: handleArea
                    hoverEnabled: true
                    anchors.fill: slider.handle
                    onPressed: function (mouse) {
                        mouse.accepted = false
                    }
                }

                // snap-back and accept logic
                PropertyAnimation on value {
                    id: snapback
                    easing.type: Easing.OutBounce
                    duration: 1250
                    to: 0
                }

                onPressedChanged: {
                    if (value === 1)
                        moveOut.start()
                    else if (!pressed)
                        snapback.start()
                }
            }

            Button {
                text: "Snooze"
                highlighted: true
                Layout.preferredHeight: parent.height
                Layout.preferredWidth: parent.width / 3
            }
        }
    }

    PropertyAnimation on x {
        easing.type: Easing.InOutQuad
        duration: 1000
        from: -width
        to: 0
    }

    SequentialAnimation on x {
        id: moveOut
        running: false

        PropertyAnimation {
            easing.type: Easing.InOutQuad
            duration: 1000
            from: 0
            to: -width
        }

        PauseAnimation {
            duration: 200
        }

        onFinished: {
            player.stop()
            dtor()
        }
    }
}
