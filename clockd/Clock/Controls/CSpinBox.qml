import QtQuick
import QtQuick.Controls

import "../Style"

CTextField {
    id: root

    property string labelText: ''
    property alias spinBox: spinBox
    placeholderText: labelText
    readOnly: true
    text: ' '

    SpinBox {
        id: spinBox
        anchors.topMargin: root.topInset + root.padding
        anchors.margins: root.padding
        anchors.fill: root

        font.pixelSize: root.font.pixelSize
        background: Item {}
    }
}
