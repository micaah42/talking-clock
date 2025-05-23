import QtQuick 2.14
import QtQuick.Controls as T
import QtQuick.Controls.Material

import Clock
import Clock.Style

T.Button {
    font.pixelSize: Theme.fontSizeMedium
    font.family: FontService.family
    Material.roundedScale: Material.ExtraSmallScale
    Material.accent: Theme.accentDark
}
