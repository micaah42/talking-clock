import QtQuick 2.15
import Clock 1.0

Item {
    id: ctrl

    property int stars
    property int blinkUp: 5000
    property int blinkDown: 1000

    Repeater {
        id: blinkers
        model: 10
        delegate: Item {}
    }

    Repeater {
        id: starsRepeater
        model: ctrl.stars
        delegate: Item {
            id: del

            x: parent.width * Math.random()
            y: parent.height * Math.random()

            Rectangle {
                id: star

                anchors.centerIn: parent
                width: 2 * Math.random() + 1.1
                height: width
                radius: width / 2

                color: Qt.lighter(ColorService.primary, 2.5)

                SequentialAnimation on opacity {
                    loops: Animation.Infinite
                    running: true

                    OpacityAnimator {
                        duration: (Math.random() + 0.5) * ctrl.blinkUp
                        from: 0.3
                        to: 0.8
                    }

                    OpacityAnimator {
                        duration: (Math.random() + 0.5) * ctrl.blinkDown
                        from: 0.8
                        to: 0.3
                    }
                }
            }
        }
    }

    // @SettingsDefinition: "clock/stars" -> stars.model
    Connections {
        id: connection
        target: SettingsService
        function onValueChanged(key, value) {
            const split = key.split('/')
            const group = split[0]
            const prop = split[1]
            if (group === "stars") {
                ctrl[prop] = value
            }
        }
    }

    Component.onCompleted: {
        SettingsService.create("stars/stars", 150)
        SettingsService.create("stars/blinkUp", 850)
        SettingsService.create("stars/blinkDown", 2500)
    }
}
