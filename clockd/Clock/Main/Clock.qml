import QtQuick
import QtQuick.Layouts 1.14

import QtQuick.Controls.Material

import Clock
import "../Style"
import "../Controls"

Item {
    id: ctrl
    property real timeScale: SpaceTheme.clockSize

    ColumnLayout {
        id: timeDisplay
        anchors.centerIn: parent
        spacing: 16
        z: 1

        CLabel {
            Layout.alignment: Qt.AlignHCenter
            text: AlarmService.now.toLocaleString(Qt.locale(), "HH:mm")
            font.pixelSize: 160 * ctrl.timeScale
            color: Theme.primary

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                height: 3
                radius: height / 2
                width: 7 * parent.width / 8
                color: Theme.accent
            }

            CLabel {
                id: seconds
                anchors.left: parent.right
                anchors.leftMargin: 16

                anchors.top: parent.top
                anchors.topMargin: 16
                text: AlarmService.now.toLocaleString(Qt.locale(), "ss")
                font.pixelSize: 64 * ctrl.timeScale
                color: Theme.primary
            }
        }

        CLabel {
            id: dateDisplay
            Layout.alignment: Qt.AlignHCenter
            text: AlarmService.now.toLocaleDateString(Qt.locale(), 'dddd, d. MMMM')
            color: Theme.primary
            font.pixelSize: 42 * ctrl.timeScale
            topPadding: -16
        }
    }
}
