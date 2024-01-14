import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQuick.Shapes 1.14

import ".."
import Clock 1.0
import Controls 1.0

Item {
    property var alarm

    Image {
        anchors.right: parent.right
        anchors.rightMargin: 24
        anchors.top: parent.top
        anchors.topMargin: 24

        sourceSize.width: 128
        sourceSize.height: 128

        source: {
            const hours = alarm.nextTrigger(new Date()).getHours()
            if (8 <= hours && hours < 22)
                return 'qrc:/sunny_FILL0_wght400_GRAD0_opsz24.svg'
            else
                return 'qrc:/clear_night_FILL0_wght400_GRAD0_opsz24.svg'
        }
    }

    ColumnLayout {
        anchors.fill: parent

        CLabel {
            text: "Next Alarm:"
            font.pixelSize: 24
        }

        CLabel {
            text: alarm.name
            font.pixelSize: 42
            bottomPadding: 16
        }

        CLabel {
            text: "Time:"
            font.pixelSize: 24
        }

        CLabel {
            text: alarm.nextTrigger(new Date()).toLocaleTimeString()
            font.pixelSize: 32
            bottomPadding: 16
        }

        CLabel {
            text: "Sound:"
            font.pixelSize: 24
        }

        CLabel {
            text: alarm.sound === '' ? 'No Sound!' : alarm.sound.split('.')[0]
            font.pixelSize: 32
            bottomPadding: 16
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
