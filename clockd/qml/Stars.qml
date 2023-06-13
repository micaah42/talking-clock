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
            width: 20
            height: width

            Rectangle {
                id: star
                property bool exploded: false
                width: 3 * Math.random() + 0.5
                height: width
                radius: width / 2
                anchors.centerIn: parent
                opacity: 0.3

                color: Qt.lighter(ColorService.primary, 2.5)
            }
            PropertyAnimation {
                id: blinkUp
                target: star
                property: "opacity"
                from: 0.3
                to: 1.0
                duration: ctrl.blinkUp
                easing.type: Easing.Linear
                onFinished: blinkDown.start()
            }
            PropertyAnimation {
                id: blinkDown
                target: star
                property: "opacity"
                from: 1.
                to: 0.3
                duration: ctrl.blinkDown
                easing.type: Easing.Linear
            }

            Connections {
                target: alarms
                function onClockTicked() {
                    if (Math.random() < 0.05 && !blinkUp.running && !blinkDown.running) {
                        blinkUp.start()
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
