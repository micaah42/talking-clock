import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import Clock 1.0
import ".."

Item {
    property var alarm

    width: window.width
    height: width / 4
    anchors.bottom: parent.bottom
    Component.onCompleted: soundService.play(alarm.sound)

    Control {
        anchors.fill: parent
        contentItem: ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20

            Label {
                text: alarm.name
                font.pixelSize: 42
                Layout.fillHeight: true
            }

            RowLayout {
                spacing: 25

                Slider {
                    id: slider
                    Layout.fillWidth: true
                    handle.height: 32
                    handle.width: 52

                    // mouse areas to restrict interactivity to handle
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onPressed: mouse.accepted = !handleArea.containsMouse
                    }

                    MouseArea {
                        id: handleArea
                        hoverEnabled: true
                        anchors.fill: slider.handle
                        onPressed: {
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
                        if (value === 1) {
                            moveOut.start()
                        }
                        if (!pressed)
                            snapback.start()
                    }
                }
                Button {
                    text: "Snooze"
                    highlighted: true
                    Layout.preferredWidth: 150
                }
            }
        }

        background: Card {
            anchors.fill: parent
            anchors.margins: 10
            opacity: 0.9
        }
    }

    PropertyAnimation on x {
        easing.type: Easing.OutQuad
        duration: 1000
        from: -window.width
        to: 0
        Component.onCompleted: this.start()
    }

    PropertyAnimation on x {
        id: moveOut
        easing.type: Easing.InQuad
        running: false
        duration: 1000
        from: 0
        to: -window.width
        onFinished: {
            soundService.stop()
        }
    }
}
