import QtQuick 2.14
import QtQuick.Controls 2.14

import Clock 1.0

CheckBox {
    property string label
    font.family: FontService.family

    CLabel {
        anchors.bottom: parent.top
        anchors.left: parent.left
        font.pixelSize: 12
        text: label
    }
}
