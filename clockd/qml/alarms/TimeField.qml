import QtQuick 2.14
import QtQml 2.0
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

import Clock 1.0
import Controls 1.0

Frame {
    id: ctrl
    Layout.fillWidth: true
    Layout.fillHeight: true

    property int visibleItems: 7
    property bool seconds: false
    property date time
    signal timeEdited

    function setTime(h, m, s) {
        var time = new Date(ctrl.time)
        if (h !== null)
            time.setHours(h)
        if (m !== null)
            time.setMinutes(m)
        if (s !== null)
            time.setSeconds(s)

        if (ctrl.time.getTime() != time.getTime()) {
            ctrl.time = time
            ctrl.timeEdited()
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
                if (!parent)
                    return 0

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
            model: 24
            delegate: delegateComponent
            currentIndex: currentIndex = ctrl.time.getHours()
            onCurrentIndexChanged: ctrl.setTime(currentIndex, null, null)
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
        }

        Text {
            Layout.preferredWidth: 1 / 5
            horizontalAlignment: Text.AlignHCenter
            text: ":"
            color: ColorService.accent
            font.pixelSize: 32
        }

        Tumbler {
            model: 60
            delegate: delegateComponent
            currentIndex: currentIndex = ctrl.time.getMinutes()
            onCurrentIndexChanged: ctrl.setTime(null, currentIndex, null)
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
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
            model: 60
            delegate: delegateComponent
            currentIndex: {
                currentIndex = ctrl.time.getSeconds()
            }
            onCurrentIndexChanged: ctrl.setTime(null, null, currentIndex)
            Layout.fillWidth: true
        }
    }
}
