import QtQuick 2.0
import Clock 1.0

Item {
    property bool bright: false
    property real backgroundOpacity: 1

    Rectangle {
        anchors.fill: parent

        color: bright ? ColorService.primary : ColorService.darkPrimary
        opacity: backgroundOpacity

        Behavior on color {
            PropertyAnimation {
                duration: 1000
            }
        }

        border.color: Qt.lighter(color, 1.15)
        border.width: 1.2
        radius: 5
    }
}
