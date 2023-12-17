import QtQuick 2.15
import Clock 1.0

Item {
    id: ctrl

    property int stars
    property int blinkUp
    property int blinkDown

    Repeater {
        id: starsRepeater
        model: ctrl.stars
        delegate: Item {
            id: del

            x: parent.width * Math.random()
            y: parent.height * Math.random()

            function blink() {
                animation.start()
            }

            width: 20
            height: width

            Rectangle {
                id: star

                width: 3 * Math.random() + 1.5
                height: width
                radius: width / 2
                anchors.centerIn: parent
                opacity: 0.3

                color: Qt.lighter(ColorService.primary, 2.5)
            }

            SequentialAnimation on opacity {
                id: animation
                PropertyAnimation {
                    to: 1.0
                    duration: ctrl.blinkUp
                    easing.type: Easing.Linear
                }
                PropertyAnimation {
                    to: 0.3
                    duration: ctrl.blinkDown
                    easing.type: Easing.Linear
                }
            }
        }
    }

    Connections {
        target: AlarmService
        function onClockTicked() {
            for (var i = 0; i < stars / 5; i++) {
                const index = Math.floor(Math.random() * stars)
                starsRepeater.itemAt(index).blink()
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
