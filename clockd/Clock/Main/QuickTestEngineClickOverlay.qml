import QtQuick

import QuickTestEngine

Item {
    id: clickOverlay
    objectName: 'ClickOverlay'

    property int easingType: Easing.InOutQuad
    property int showDuration: 100
    property int hideDuration: 750
    property real maxOpacity: 0.5
    property real s: 128
    z: 1

    Component {
        id: markerComponent

        Item {
            id: marker

            Rectangle {
                id: r

                x: -s / 2
                y: -s / 2
                radius: s / 2
                height: s
                width: s

                SequentialAnimation {
                    onFinished: marker.destroy(10)
                    running: true

                    ParallelAnimation {
                        OpacityAnimator {
                            easing.type: easingType
                            duration: showDuration
                            from: 0
                            to: maxOpacity
                            target: r
                        }
                        ScaleAnimator {
                            easing.type: easingType
                            duration: showDuration
                            from: 0
                            to: 0.5
                            target: r
                        }
                    }
                    ParallelAnimation {
                        OpacityAnimator {
                            easing.type: easingType
                            duration: hideDuration
                            from: maxOpacity
                            to: 0
                            target: r
                        }
                        ScaleAnimator {
                            easing.type: easingType
                            duration: hideDuration
                            from: 0.5
                            to: 0
                            target: r
                        }
                    }
                }
            }
        }
    }

    Connections {
        target: QuickTestEngine

        function onMousePressed(point) {
            const properties = {
                "x": point.x,
                "y": point.y
            }

            markerComponent.createObject(clickOverlay, properties)
        }
    }
}
