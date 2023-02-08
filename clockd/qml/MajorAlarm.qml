import QtQuick 2.14
import QtQuick.Controls 2.14

// import Clock 1.0
Item {
    id: ctrl
    z: 100
    property var alarm
    signal accepted

    Item {
        x: -window.width
        Rectangle {
            id: bg
            width: 1.92 * window.width
            height: width
            radius: width / 2
            x: -width / 2
            y: -width / 2
            color: colorService.colors["darkaccent"]
            border.color: "#fff"
            border.width: 2
        }

        PropertyAnimation on x {
            id: popper
            running: true
            easing.overshoot: 0.4
            easing.amplitude: 0.2
            from: -window.width
            to: 0

            easing.type: Easing.OutBack
            duration: 1750
        }

        PropertyAnimation on x {
            id: closer
            running: false
            easing.overshoot: 0.4
            easing.amplitude: 0.2
            from: 0
            to: -window.width

            easing.type: Easing.OutBack
            duration: 1750

            onFinished: {
                console.log('alarm accepted!')
                ctrl.accepted()
                ctrl.destroy()
            }
        }

        Column {
            id: description
            x: parent.width / 2 + 42
            y: 30
            width: ctrl.width / 2
            spacing: 10

            Text {
                text: alarm["name"]
                font.pixelSize: 82
            }
            Text {
                text: new Date(alarm["time"]).toLocaleTimeString()
                font.pixelSize: 42
                color: "#ffffff"
            }

            Rectangle {
                width: parent.width
                height: 100
                radius: 5
                color: Qt.lighter(colorService.colors["darkaccent"], 0.8)

                ScrollView {
                    id: chatBox
                    anchors.margins: 5
                    anchors.fill: parent
                    spacing: 5
                    TextArea {
                        text: "here i will\nplace output\nfrom commands!\nyeaah :)"
                    }
                }
            }
            Button {
                width: parent.width
                highlighted: true
                text: "Accept"
                property bool wasHeld: false
                onPressAndHold: {
                    wasHeld = true
                }
                onReleased: {
                    if (!wasHeld)
                        return

                    soundService.stop()
                    closer.start()
                }
            }

            Component.onCompleted: {
                console.log('play', alarm["sound"])
                soundService.play(alarm['sound'])
            }
        }
    }
}
