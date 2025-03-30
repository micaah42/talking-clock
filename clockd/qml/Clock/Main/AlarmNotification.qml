import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtMultimedia

import Clock 1.0
import Clock.Controls 1.0
import Clock.Style 1.0

ColumnLayout {
    id: root
    property Alarm alarm
    property bool accepted: false
    spacing: 8

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
                loops: 2

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

    RowLayout {
        spacing: 4
        ValueDisplay {
            Layout.preferredWidth: parent.width / 2
            visible: alarm.name !== ''
            labelText: 'Name'
            valueText: alarm.name
        }
        Rectangle {
            visible: alarm.name !== ''
            Layout.fillHeight: true
            Layout.preferredWidth: 1
            Layout.rightMargin: 4
            Layout.leftMargin: 4
            opacity: 0.2
        }
        ValueDisplay {
            Layout.preferredWidth: parent.width / 2
            labelText: 'Time'
            valueText: alarm.time.toLocaleTimeString()
        }
    }

    Rectangle {
        Layout.preferredHeight: 1
        Layout.fillWidth: true
        Layout.bottomMargin: 8
        Layout.topMargin: 8
        opacity: 0.2
    }

    ActionDayItem {
        Layout.fillHeight: true
        Layout.fillWidth: true
        clip: true
    }

    DelayButton {
        id: bttn

        Layout.preferredHeight: 88
        Layout.fillWidth: true
        delay: accepted ? 0 : 850

        onClicked: player.stop()

        onActivated: {
            accepted = true
            player.stop()
        }

        onReleased: {
            if (accepted)
                root.StackView.view.pop()
        }

        background: Rectangle {
            anchors.fill: parent

            border.width: 3.5
            SequentialAnimation on border.color {
                loops: Animation.Infinite
                running: !bttn.pressed && !bttn.wasActivated

                ColorAnimation {
                    from: 'white'
                    to: Theme.accent
                    easing.type: Easing.InOutQuad
                    duration: 1000
                }
                ColorAnimation {
                    from: Theme.accent
                    to: 'white'
                    easing.type: Easing.InOutQuad
                    duration: 1000
                }
            }

            radius: height / 4
            gradient: Gradient {
                orientation: Qt.Horizontal

                GradientStop {
                    color: Theme.accent
                }
                GradientStop {
                    color: Theme.accentColor(Material.Shade300)

                    SequentialAnimation on position {
                        loops: Animation.Infinite

                        PropertyAnimation {
                            easing.type: Easing.InOutQuad
                            duration: 1000
                            from: 1e-6
                            to: 1 - 1e-6
                        }

                        PropertyAnimation {
                            easing.type: Easing.InOutQuad
                            duration: 1000
                            from: 1 - 1e-6
                            to: 1e-6
                        }
                    }
                }
                GradientStop {
                    color: Theme.accent
                    position: 1
                }
            }
        }

        contentItem: Item {
            anchors.fill: parent

            Rectangle {
                id: wiggly

                anchors.fill: parent
                anchors.margins: 16 * (1 - bttn.progress) + 2

                border.color: Theme.primaryColor(Material.Shade300)
                border.width: 3.5

                color: bttn.pressed ? Qt.darker(Theme.primary, 1.2) : Theme.primary
                radius: height / 4

                CLabel {
                    anchors.centerIn: parent
                    font.family: FontService.family
                    font.pixelSize: 32
                    text: 'OK'
                }
            }
        }
    }

    Button {
        onClicked: {
            alarmStack.pop()
            alarm.snooze(5)
            player.stop()
        }

        Material.roundedScale: Material.LargeScale
        Material.background: Theme.accent
        Layout.preferredHeight: 64
        Layout.fillWidth: true
        font.pixelSize: 32
        text: "Snooze"
    }

    Timer {
        onTriggered: root.StackView.view.pop()
        interval: 15 * 60000
        running: true
    }
}
