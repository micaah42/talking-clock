import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import Clock 1.0
import Clock.Style 1.0
import Clock.Controls 1.0

Button {
    property Component comp
    property string name

    readonly property var page: {
        "comp": comp,
        "icon": text,
        "name": name
    }

    Material.roundedScale: Material.SmallScale
    Material.background: Theme.primaryDark
    implicitWidth: 0
    text: icon

    font.family: Icons.fontFamily
    font.pixelSize: 88
    bottomInset: 0
    topInset: 0
}
