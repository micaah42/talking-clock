import QtQuick 2.14
import QtQml 2.0
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import "qrc:/controls"


Frame {
    id: ctrl
    Layout.fillWidth: true
    Layout.fillHeight: true
    property date time
    signal timeEdited()

    function setTime(h, m, s) {
        var time = new Date(ctrl.time);
        if (h !== null)
            time.setHours(h);
        if (m !== null)
            time.setMinutes(m);
        if (s !== null)
            time.setSeconds(s);

        if (ctrl.time.getTime() != time.getTime()) {
            ctrl.time = time;
            ctrl.timeEdited();
        }
    }

    RowLayout {
        id: row
        anchors.fill: parent

        Component {
            id: delegateComponent

            CLabel {
                text: modelData.toString().padStart(2, "0")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: {
                    var displacement = Math.abs(Tumbler.displacement);
                    var visibleItems = Tumbler.tumbler.visibleItemCount;
                    if (displacement < 1)
                        return 64 - 32 * displacement;
                    else
                        return 32
                }
                font.family: fontService.families[fontService.currentFamilyIndex]
            }
        }

        Tumbler {
            id: hoursTumbler
            model: 24
            delegate: delegateComponent
            currentIndex: { currentIndex = ctrl.time.getHours() }
            onCurrentIndexChanged: ctrl.setTime(currentIndex, null, null);
            Layout.fillWidth: true
        }

        Text {
            Layout.preferredWidth: 1 / 5
            horizontalAlignment: Text.AlignHCenter
            text: ":"
            color: colorService.colors["accent"]
            font.pixelSize: 32
        }

        Tumbler {
            model: 60
            delegate: delegateComponent
            currentIndex: { currentIndex = ctrl.time.getMinutes() }
            onCurrentIndexChanged: ctrl.setTime(null, currentIndex, null)
            Layout.fillWidth: true
        }

        Text {
            Layout.preferredWidth: 1 / 5
            horizontalAlignment: Text.AlignHCenter
            text: ":"
            color: colorService.colors["accent"]
            font.pixelSize: 32
        }

        Tumbler {
            model: 60
            delegate: delegateComponent
            currentIndex: { currentIndex = ctrl.time.getMinutes() }
            onCurrentIndexChanged: ctrl.setTime(null, null, currentIndex)
            Layout.fillWidth: true
        }
    }
}
