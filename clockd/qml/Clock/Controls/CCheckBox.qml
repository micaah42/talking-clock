import QtQuick 2.14
import QtQuick.Controls 2.14

import Clock 1.0

CheckBox {
    property alias labelText: l.text
    property alias label: l

    font.family: FontService.family

    CLabel {
        id: l
        anchors.leftMargin: parent.leftPadding
        anchors.bottom: parent.top
        anchors.left: parent.left
        anchors.topMargin: 4
        font.pixelSize: 18
    }
}
