import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import Clock
import "../Style"

Button {
    Material.roundedScale: Material.ExtraSmallScale
    Material.accent: Theme.accentDark

    font.pixelSize: Theme.fontSizeMedium
    font.family: FontService.family
}
