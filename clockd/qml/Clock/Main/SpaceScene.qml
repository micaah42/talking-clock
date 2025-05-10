import QtQuick 2.15
import QtQuick.Controls.Material

import Clock 1.0
import Clock.Style 1.0
import QtQuick.Effects

Item {
    id: root

    property int padding: 64
    property int radius: 32

    readonly property var shades: [Material.Shade50, Material.Shade100]

    function randomShade() {
        return shades[Math.ceil(Math.random() * shades.length)]
    }

    function recalculateSpaceShip() {
        rocketAnimation.start()
        rocketAnimation.stop()
    }

    onHeightChanged: recalculateSpaceShip()
    onWidthChanged: recalculateSpaceShip()

    Repeater {
        id: starsRepeater
        model: 5
        delegate: Item {
            id: d
            anchors.fill: parent

            Repeater {
                model: SpaceTheme.stars / starsRepeater.count
                delegate: Rectangle {
                    y: parent.height * Math.random() - height / 2
                    x: parent.width * Math.random() - width / 2

                    color: Material.color(Material.Indigo, randomShade())
                    width: 5.5 * Math.random() + 1.75
                    radius: width / 2
                    height: width
                }
            }

            property real minOpacity: Math.random() * 0.5
            property real maxOpacity: minOpacity + Math.random() * 0.5

            SequentialAnimation on opacity {
                loops: Animation.Infinite

                OpacityAnimator {
                    easing.type: Easing.InOutElastic
                    duration: index * SpaceTheme.animationSpeed
                    from: d.minOpacity
                    to: d.maxOpacity
                }
                OpacityAnimator {
                    easing.type: Easing.InOutElastic
                    duration: index * SpaceTheme.animationSpeed
                    from: d.maxOpacity
                    to: d.minOpacity
                }
            }
        }
    }

    Item {
        id: container
        anchors.fill: parent

        SequentialAnimation {
            loops: Animation.Infinite
            running: true

            PathAnimation {
                id: rocketAnimation
                target: rocket
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
