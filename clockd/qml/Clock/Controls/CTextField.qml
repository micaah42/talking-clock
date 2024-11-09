import QtQuick 2.14
import QtQuick.Controls 2.14

import Clock 1.0

TextField {
    id: input
    property alias labelText: l.text
    property alias label: l

    onAccepted: Qt.inputMethod.hide()
    font.family: FontService.family
    implicitHeight: 56
    font.pixelSize: 24
    leftPadding: 12
    bottomPadding: 0
    topPadding: 16

    implicitWidth: 244
    width: implicitWidth

    CLabel {
        id: l

        anchors.leftMargin: parent.leftPadding
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 8

        background: Item {}
        font.pixelSize: 14
    }
}
