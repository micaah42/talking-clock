import QtQuick 2.15
import QtQuick.Controls.Material

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

    readonly property var shades: [Material.Shade50, Material.Shade100]

    function randomShade() {
        return shades[Math.ceil(Math.random() * shades.length)]
    }

    Repeater {
        id: starsRepeater
        model: 15
        delegate: Item {
            anchors.fill: parent

            Repeater {
                model: 5 // SpaceScene.blinkingCount
                delegate: Rectangle {
                    y: parent.height * Math.random() - height / 2
                    x: parent.width * Math.random() - width / 2

                    color: Material.color(Material.Indigo, randomShade())
                    width: 3.5 * Math.random() + 1.5
                    radius: width / 2
                    height: width
                }
            }

            property real maxOpacity: 0.75 + 0.2 * Math.random()
            property real minOpacity: 0.25

            SequentialAnimation on opacity {
                loops: Animation.Infinite

                OpacityAnimator {
                    duration: index * root.blinkUp
                    from: .25
                    to: .85
                }
                OpacityAnimator {
                    duration: index * root.blinkUp
                    from: .85
                    to: .25
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
