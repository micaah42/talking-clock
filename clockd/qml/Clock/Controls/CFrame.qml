import QtQuick
import QtQuick.Controls

import Clock.Style

Frame {
    property alias backgroundColor: rectangle.color

    background: Rectangle {
        id: rectangle
        border.color: Material.frameColor
        color: 'transparent'
        radius: 7
    }
}
