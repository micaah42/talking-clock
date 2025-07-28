import QtQuick
import QtQuick.Layouts

import Clock
import "../Style"
import "../Controls"

RowLayout {
    property int msecs

    Component {
        id: comp
        ColumnLayout {
            id: col
            Layout.preferredWidth: 120
            required property string title
            required property int duration
            required property int max

            CLabel {
                Layout.alignment: Qt.AlignHCenter
                opacity: Theme.o72
                text: col.title
            }
            Button {
                onClicked: msecs += col.duration
                font.pixelSize: Theme.fontSizeLarge
                Layout.fillWidth: true
                autoRepeat: true

                text: '+'
            }
            CLabel {
                Layout.alignment: Qt.AlignHCenter
                text: Math.floor(msecs / col.duration) % max
                Layout.fillHeight: true
                size: CLabel.XLarge
            }
            Button {
                onClicked: msecs -= col.duration
                enabled: msecs >= col.duration
                font.pixelSize: Theme.fontSizeLarge
                Layout.fillWidth: true
                autoRepeat: true
                text: '-'
            }
        }
    }

    Repeater {
        id: repeater
        model: ListModel {
            ListElement {
                title: 'Days'
                duration: 86400000
                max: -1
            }
            ListElement {
                title: 'Hours'
                duration: 3600000
                max: 24
            }
            ListElement {
                title: 'Minutes'
                duration: 60000
                max: 60
            }
            ListElement {
                title: 'Seconds'
                duration: 1000
                max: 60
            }
        }

        delegate: comp
    }
}
