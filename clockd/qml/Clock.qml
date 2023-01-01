import QtQuick 2.14
import QtQuick.Layouts 1.14
import "qrc:/controls"

Item {
    id: ctrl
    property int stars;
    property real timeScale;
    property int blinkUp;
    property int blinkDown;

    ColumnLayout {
        id: timeDisplay
        z: 10
        anchors.centerIn: parent
        CLabel {
            text: new Date(alarms.clockString).toLocaleString(Qt.locale(), "HH:mm")
            color: colorService.colors["primary"]
            font.pixelSize: 150 * ctrl.timeScale
        }
        Rectangle {
            Layout.preferredWidth: parent.width - 20
            Layout.preferredHeight: 3
            radius: height / 2
            Layout.alignment: Qt.AlignHCenter
            color: colorService.colors["accent"]
        }
        CLabel {
            id: dateDisplay
            text: new Date(alarms.clockString).toLocaleDateString(Qt.locale())
            Layout.alignment: Qt.AlignHCenter
            color: colorService.colors["primary"]
            font.pixelSize: 30 * ctrl.timeScale
        }
    }

    CLabel {
        anchors.top: timeDisplay.top
        anchors.left: timeDisplay.right
        text: new Date(alarms.clockString).toLocaleString(Qt.locale(), "ss")
        color: colorService.colors["primary"]
        font.pixelSize: 50 * ctrl.timeScale
    }

    // stars
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

                color: Qt.lighter(colorService.colors["primary"], 2.5)
            }
            PropertyAnimation {
                id: blinkUp
                target: star
                property: "opacity"
                from: 0.3; to: 1.0
                duration: ctrl.blinkUp
                easing.type: Easing.Linear
                onFinished: blinkDown.start()
            }
            PropertyAnimation {
                id: blinkDown
                target: star
                property: "opacity"
                from: 1.; to: 0.3
                duration: ctrl.blinkDown
                easing.type: Easing.Linear
            }

            Connections {
                target: alarms
                function onClockTicked() {
                    if (Math.random() < 0.05 && !blinkUp.running && !blinkDown.running) {
                        blinkUp.start();
                    }
                }
            }
        }
    }

    // @SettingsDefinition: "clock/stars" -> stars.model
    Connections {
        id: connection
        target: settingsService
        function onValueChanged(key, value) {
            var [group, prop] = key.split('/');
            if (group === "clock") {
                ctrl[prop] = value;
            }
        }
    }
    Component.onCompleted: {
        settingsService.create("clock/stars", 150);
        settingsService.create("clock/blinkUp", 850);
        settingsService.create("clock/blinkDown", 2500);
        settingsService.create("clock/timeScale", 1.4);
    }
}
