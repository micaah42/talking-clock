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
        spacing: 16
        z: 10

        RowLayout {
            Layout.alignment: Qt.AlignHCenter

            Item {
                Layout.preferredWidth: seconds.width // compensate ss to center hh:mm
            }

            CLabel {
                text: AlarmService.now.toLocaleString(Qt.locale(), "HH:mm")
                font.pixelSize: 150 * ctrl.timeScale
                color: ColorService.primary

                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 20
                    height: 3
                    radius: height / 2
                    width: parent.width
                    color: ColorService.accent
                }
            }

            CLabel {
                id: seconds
                Layout.alignment: Qt.AlignTop
                text: AlarmService.now.toLocaleString(Qt.locale(), "ss")
                font.pixelSize: 50 * ctrl.timeScale
                color: ColorService.primary
            }
        }

        CLabel {
            id: dateDisplay
            Layout.alignment: Qt.AlignHCenter
            text: AlarmService.now.toLocaleDateString(Qt.locale(), 'dddd, d. MMMM')
            font.pixelSize: 36 * ctrl.timeScale
            color: Qt.lighter(ColorService.primary, 0.8)
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
