import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQuick.Shapes 1.14

import ".."
import Clock 1.0
import Controls 1.0

Frame {
    property var alarm: {
        if (AlarmService.nextIds.length === 0)
            return null
        if (AlarmService.model.size() <= AlarmService.nextIds[0])
            return null
        return AlarmService.model.at(AlarmService.nextIds[0])
    }

    ColumnLayout {
        CLabel {
            text: "Next Alarm:"
            font.pixelSize: 24
        }

        CLabel {
            text: alarm ? alarm.name : ' '
            font.pixelSize: 42
            bottomPadding: 16
        }

        CLabel {
            text: "Time:"
            font.pixelSize: 24
        }

        CLabel {
            text: alarm ? alarm.nextTrigger(new Date()).toLocaleTimeString() : ' '
            font.pixelSize: 32
            bottomPadding: 16
        }

        CLabel {
            text: "Sound:"
            font.pixelSize: 24
        }

        CLabel {
            text: alarm ? alarm.sound : ' '
            font.pixelSize: 32
            bottomPadding: 16
        }

        Item {
            Layout.fillHeight: true
        }
    }

    clip: true

    Shape {
        id: line
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.margins: -40
        asynchronous: true
        height: 40

        property real t

        NumberAnimation on t {
            loops: Animation.Infinite
            running: true
            from: 0
            to: 2 * Math.PI
            duration: 3500
        }

        ShapePath {
            startX: 0
            startY: 0
            strokeWidth: 3
            strokeColor: ColorService.accent
            fillColor: 'transparent'

            PathPolyline {
                property int n: 36

                path: {
                    const piScale = 5 * Math.PI / n
                    const xScale = line.width / n
                    const yScale = line.height / 2
                    const p = []

                    for (var i = 0; i < n; i++) {
                        p.push(Qt.point(xScale * i, yScale * Math.sin(line.t + piScale * i)))
                    }

                    return p
                }
            }
        }
    }
}
