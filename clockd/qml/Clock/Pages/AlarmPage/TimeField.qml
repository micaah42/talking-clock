import QtQuick 2.14
import QtQml 2.0
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

import Clock 1.0
import Clock.Controls 1.0

Frame {
    id: ctrl
    Layout.fillWidth: true
    Layout.fillHeight: true

    property int minuteStepSize: 5
    property int secondStepSize: 5
    property int visibleItems: 9
    property bool seconds: false

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

            font.pixelSize: {
                var displacement = 2 * Math.abs(Tumbler.displacement) / visibleItems
                return Math.max(64 - 32 * displacement, 24)
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

        Text {
            Layout.preferredWidth: 1 / 5
            horizontalAlignment: Text.AlignHCenter
            text: ":"
            color: ColorService.accent
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

        Text {
            visible: seconds
            Layout.preferredWidth: 1 / 5
            horizontalAlignment: Text.AlignHCenter
            text: ":"
            color: ColorService.accent
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
            Label {
                Layout.alignment: Qt.AlignRight
                opacity: minuteStepSize === 5 ? 1 : 0.5
                font.pixelSize: 24
                text: '5m'

                MouseArea {
                    anchors.fill: parent
                    onClicked: ctrl.minuteStepSize = 5
                }
            }
            Label {
                Layout.alignment: Qt.AlignRight
                opacity: minuteStepSize === 1 ? 1 : 0.5
                font.pixelSize: 24
                text: '1m'

                MouseArea {
                    anchors.fill: parent
                    onClicked: ctrl.minuteStepSize = 1
                }
            }
        }
    }
}
