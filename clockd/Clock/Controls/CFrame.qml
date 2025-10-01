import QtQuick
import QtQuick.Controls

import "../Style"

Frame {
    property alias backgroundColor: rectangle.color

    rightPadding: 24
    leftPadding: 24
    padding: 16

    background: Rectangle {
        id: rectangle
        border.color: Material.frameColor
        border.width: 1.5
        color: 'transparent'
        radius: 7
    }
}
