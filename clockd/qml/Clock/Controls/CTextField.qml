import QtQuick 2.14
import QtQuick.Controls 2.14

import Clock 1.0

TextField {
    id: input
    property alias label: l.text

    onAccepted: Qt.inputMethod.hide()
    font.family: FontService.family
    implicitHeight: 55
    font.pixelSize: 20
    leftPadding: 12
    topPadding: 16

    CLabel {
        id: l

        anchors.leftMargin: parent.leftPadding
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 8

        background: Item {}
        font.pixelSize: 18
    }
}
