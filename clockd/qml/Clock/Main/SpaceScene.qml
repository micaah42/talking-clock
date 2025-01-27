import QtQuick 2.15

import Clock 1.0
import Clock.Style 1.0
import QtQuick.Effects

Item {
    id: root

    property int blinkingCount: SpaceTheme.animatedStars
    property int blinkDown: SpaceTheme.animationSpeed
    property int blinkUp: SpaceTheme.animationSpeed
    property int stars: SpaceTheme.stars
    property int padding: 64
    property int radius: 32

    Repeater {
        model: stars
        delegate: Rectangle {

            x: parent.width * Math.random()
            y: parent.height * Math.random()
            opacity: 0.4 + 0.2 * Math.random()

            color: Qt.lighter(ColorService.primary, 2.5)
            width: 2 * Math.random() + 1.1
            radius: width / 2
            height: width
        }
    }

    Repeater {
        id: starsRepeater
        model: 5
        delegate: Repeater {
            model: Math.ceil(SpaceScene.blinkingCount / 5)
            delegate: Item {
                id: del

                property real maxOpacity: 0.6 + 0.2 * Math.random()
                property real minOpacity: 0.2

                x: parent.width * Math.random()
                y: parent.height * Math.random()

                Rectangle {
                    anchors.centerIn: parent
                    width: 2 * Math.random() + 1.1
                    height: width
                    radius: width / 2

                    color: Qt.lighter(ColorService.primary, 2.5)

                    SequentialAnimation on opacity {
                        loops: Animation.Infinite

                        PauseAnimation {
                            duration: Math.random() * root.blinkUp
                        }
                        OpacityAnimator {
                            duration: root.blinkUp
                            from: 0
                            to: 1
                        }
                        OpacityAnimator {
                            duration: root.blinkUp
                            from: 1
                            to: 0
                        }
                    }
                }
            }
        }
    }

    Item {
        id: container
        anchors.fill: parent

        // y: 24
        SequentialAnimation {
            loops: Animation.Infinite
            running: true

            PathAnimation {
                target: rocket
                //anchorPoint: Qt.point(0.5, 0.5)
                orientation: PathAnimation.TopFirst
                duration: 10000

                path: Path {
                    id: path

                    startX: container.width / 2
                    startY: container.height + 64

                    PathLine {
                        x: container.width / 2
                        y: container.height - padding + radius
                    }
                    PathArc {
                        direction: PathArc.Counterclockwise
                        relativeX: -radius
                        relativeY: -radius
                        radiusX: -radius
                        radiusY: -radius
                    }
                    PathLine {
                        x: padding + radius
                        y: container.height - padding
                    }
                    PathArc {
                        relativeX: -radius
                        relativeY: -radius
                        radiusX: -radius
                        radiusY: -radius
                    }
                    PathLine {
                        x: padding
                        y: padding + radius
                    }
                    PathArc {
                        relativeX: radius
                        relativeY: -radius
                        radiusX: radius
                        radiusY: -radius
                    }
                    PathLine {
                        x: container.width - padding - radius
                        y: padding
                    }
                    PathArc {
                        relativeX: radius
                        relativeY: radius
                        radiusX: radius
                        radiusY: radius
                    }
                    PathLine {
                        x: container.width - padding
                        y: container.height - padding - radius
                    }
                    PathArc {
                        relativeX: -radius
                        relativeY: radius
                        radiusX: -radius
                        radiusY: radius
                    }
                    PathLine {
                        x: container.width / 2 + radius
                        y: container.height - padding
                    }
                    PathArc {
                        direction: PathArc.Counterclockwise
                        relativeX: -radius
                        relativeY: radius
                        radiusX: -radius
                        radiusY: radius
                    }
                    PathLine {
                        x: container.width / 2
                        y: container.height + 64
                    }
                }
            }

            PauseAnimation {
                duration: 15 * 60000
            }
        }
        Item {
            id: rocket

            Icon {
                id: image
                anchors.centerIn: parent
                text: Icons.rocket_launch
                font.pixelSize: 32
                rotation: -45
                z: 1
            }
        }
    }
}
