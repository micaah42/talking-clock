import QtQuick 2.14
import QtQuick.Layouts 1.14

import Clock 1.0
import Controls 1.0

Item {
    id: ctrl
    property real timeScale

    ColumnLayout {
        id: timeDisplay
        anchors.centerIn: parent
        z: 10

        RowLayout {
            Item {
                Layout.preferredWidth: seconds.width // compensate ss to center hh:mm
            }
            CLabel {
                text: new Date(alarms.clockString).toLocaleString(Qt.locale(), "HH:mm")
                color: ColorService.primary
                font.pixelSize: 150 * ctrl.timeScale
            }
            CLabel {
                id: seconds
                Layout.alignment: Qt.AlignTop
                text: new Date(alarms.clockString).toLocaleString(Qt.locale(), "ss")
                color: ColorService.primary
                font.pixelSize: 50 * ctrl.timeScale
            }
        }

        Rectangle {
            Layout.preferredHeight: 3
            Layout.preferredWidth: dateDisplay.width
            radius: height / 2
            Layout.alignment: Qt.AlignHCenter
            color: ColorService.accent
        }
        CLabel {
            id: dateDisplay
            text: new Date(alarms.clockString).toLocaleDateString(Qt.locale())
            Layout.alignment: Qt.AlignHCenter
            color: ColorService.darkPrimary
            font.pixelSize: 30 * ctrl.timeScale
            style: Text.Outline
            styleColor: ColorService.primary
        }
    }

    Connections {
        id: connection
        target: SettingsService
        function onValueChanged(key, value) {
            const split = key.split('/')
            const group = split[0]
            const prop = split[1]
            if (group === "clock") {
                ctrl[prop] = value
            }
        }
    }

    Component.onCompleted: {
        SettingsService.create("clock/timeScale", 1.4)
    }
}
