import QtQuick 2.15
import QtQuick.Effects
import QtQuick.Controls.Material

import Clock

import "../Style"

Item {
    id: root

    property int padding: 64
    property int radius: 32

    readonly property int maxLayers: 8
    function randomShade() {
        return shades[Math.ceil(Math.random() * shades.length)]
    }

    onHeightChanged: rocketAnimation.restart()
    onWidthChanged: rocketAnimation.restart()

    Repeater {
        id: animatedStars
        model: SpaceTheme.animatedStars
        delegate: Item {
            id: d
            anchors.fill: parent
            property int i: index

            Item {
                anchors.fill: parent
                layer.enabled: true

                Repeater {
                    id: staticStars
                    model: Math.floor(SpaceTheme.stars / animatedStars.count)

                    delegate: Image {
                        id: stars
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectCrop
                        source: `qrc:/space-theme/stars${d.i * staticStars.count + index}.svg`
                    }
                }
            }

            property real maxOpacity: Math.random() * Theme.o24 + Theme.o72
            property real minOpacity: Theme.o24

            SequentialAnimation on opacity {
                loops: Animation.Infinite

                OpacityAnimator {
                    easing.type: Easing.OutBounce
                    duration: (index + 5) * SpaceTheme.animationSpeed
                    from: d.minOpacity
                    to: d.maxOpacity
                }
                OpacityAnimator {
                    easing.type: Easing.InBounce
                    duration: (index + 8) * SpaceTheme.animationSpeed
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
            id: rocketAnimation
            loops: Animation.Infinite
            running: true

            PathAnimation {
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
