import QtQuick 2.15

import Clock 1.0

Item {
    id: ctrl

    property int blinkingCount: SpaceTheme.animatedStars
    property int blinkDown: SpaceTheme.animationSpeed
    property int blinkUp: SpaceTheme.animationSpeed
    property int stars: SpaceTheme.stars

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
        model: blinkingCount
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
                        duration: Math.random() * ctrl.blinkUp
                    }
                    OpacityAnimator {
                        duration: ctrl.blinkUp
                        from: 0
                        to: 1
                    }
                    OpacityAnimator {
                        duration: ctrl.blinkUp
                        from: 1
                        to: 0
                    }
                }
            }
        }
    }
}
