import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import Clock
import "../Style"

MenuItem {
    id: root

    contentItem: CLabel {
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: Theme.fontSizeLarge
        text: root.text
    }
}
