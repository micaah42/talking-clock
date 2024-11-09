import QtQuick 2.14
import QtQuick.Controls 2.14

import Clock 1.0

CheckBox {
    property alias labelText: l.text
    property alias label: l

    font.family: FontService.family
    implicitHeight: 56
    leftPadding: 0
    topPadding: 0

    CLabel {
        id: l
        anchors.top: parent.top
        anchors.leftMargin: 2
        anchors.left: parent.left
        anchors.topMargin: 4
        font.pixelSize: 14
    }
}
