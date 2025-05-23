import QtQuick
import QtQuick.Controls

import Clock
import Clock.Style

TextField {
    id: input
    property alias labelText: l.text
    property alias label: l

    onAccepted: Qt.inputMethod.hide()
    font.family: FontService.family

    implicitWidth: Theme.controlWidth
    width: implicitWidth
    font.pixelSize: 24
    topInset: 8

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
