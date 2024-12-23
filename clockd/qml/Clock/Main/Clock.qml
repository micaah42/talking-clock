import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Effects

import Clock 1.0
import Clock.Controls 1.0

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
            color: ColorService.primary
            bottomPadding: -24

            Rectangle {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                height: 3
                radius: height / 2
                width: parent.width
                color: ColorService.accent
            }

            CLabel {
                id: seconds
                anchors.left: parent.right
                anchors.leftMargin: 16

                anchors.top: parent.top
                anchors.topMargin: 16
                text: AlarmService.now.toLocaleString(Qt.locale(), "ss")
                font.pixelSize: 64 * ctrl.timeScale
                color: ColorService.primary
            }
        }

        CLabel {
            id: dateDisplay
            Layout.alignment: Qt.AlignHCenter
            text: AlarmService.now.toLocaleDateString(Qt.locale(), 'dddd, d. MMMM')
            font.pixelSize: 42 * ctrl.timeScale
            color: ColorService.primary
            topPadding: -24
        }
    }
}
