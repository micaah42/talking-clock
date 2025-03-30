import QtQuick 2.14
import QtQml 2.0
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

import Clock 1.0
import Clock.Style
import Clock.Controls 1.0

Frame {
    id: ctrl
    Layout.fillWidth: true
    Layout.fillHeight: true

    enum Mode {
        M5,
        M1,
        S1
    }

    property int mode: TimeField.Mode.M5
    property int visibleItems: 9
    readonly property int secondStepSize: mode === TimeField.S1 ? 1 : 60
    readonly property int minuteStepSize: mode === TimeField.M5 ? 5 : 1
    readonly property bool seconds: mode === TimeField.S1

    property date time

    onTimeChanged: {
        console.log('time changed', time)

        if (isNaN(time.getTime())) {
            console.warn('invalid time set, fixing setting now..')
            time = new Date()
        }
    }

    Component {
        id: delegateComponent

        CLabel {
            text: modelData.toString().padStart(2, "0")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: FontService.family
            property Tumbler tumbler: Tumbler.tumbler

            font.pixelSize: {
                var displacement = 2 * Math.abs(Tumbler.displacement) / visibleItems
                return Math.max(64 - 32 * displacement, 24)
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    parent.tumbler.positionViewAtIndex(index, Tumbler.Center)
                    parent.tumbler.currentIndex = index
                }
            }
        }
    }

    RowLayout {
        id: row
        anchors.fill: parent

        Tumbler {
            id: hoursTumbler
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true

            model: 24
            delegate: delegateComponent

            currentIndex: currentIndex = time.getHours()
            onCurrentIndexChanged: {
                time = new Date(time.setHours(currentIndex))
                console.log('new hours:', currentIndex, time)
            }
        }

        CLabel {
            Layout.preferredWidth: 1 / 5
            horizontalAlignment: Text.AlignHCenter
            text: ":"
            color: Theme.accent
            font.pixelSize: 32
        }

        Tumbler {
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true

            model: [...Array(60 / minuteStepSize).keys()].map(x => minuteStepSize * x)
            delegate: delegateComponent

            currentIndex: currentIndex = Math.round(time.getMinutes() / minuteStepSize)
            onCurrentIndexChanged: {
                time = new Date(time.setMinutes(model[currentIndex]))
                console.log('new minutes:', model[currentIndex], time)
            }
        }

        CLabel {
            visible: seconds
            Layout.preferredWidth: 1 / 5
            horizontalAlignment: Text.AlignHCenter
            text: ":"
            color: Theme.accent
            font.pixelSize: 32
        }

        Tumbler {
            visible: seconds

            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true

            model: [...Array(60 / secondStepSize).keys()].map(x => secondStepSize * x)
            delegate: delegateComponent

            currentIndex: currentIndex = Math.round(time.getSeconds() / secondStepSize)
            onCurrentIndexChanged: {
                time = new Date(time.setSeconds(model[currentIndex]))
                console.log('new seconds:', model[currentIndex], time)
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignTop
            CLabel {
                Layout.alignment: Qt.AlignRight
                opacity: mode === TimeField.M5 ? 1 : 0.5
                font.pixelSize: 24
                text: '5m'

                MouseArea {
                    anchors.fill: parent
                    onClicked: mode = TimeField.M5
                }
            }
            CLabel {
                Layout.alignment: Qt.AlignRight
                opacity: mode === TimeField.M1 ? 1 : 0.5
                font.pixelSize: 24
                text: '1m'

                MouseArea {
                    anchors.fill: parent
                    onClicked: mode = TimeField.M1
                }
            }
            CLabel {
                Layout.alignment: Qt.AlignRight
                opacity: mode === TimeField.S1 ? 1 : 0.5
                font.pixelSize: 24
                text: '1s'

                MouseArea {
                    anchors.fill: parent
                    onClicked: mode = TimeField.S1
                }
            }
        }
    }
}
