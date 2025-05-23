import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Clock.Style

RowLayout {
    property alias labelText: l.text
    property alias label: l

    property alias switchItem: switchItem
    property alias checked: switchItem.checked

    implicitWidth: Theme.controlWidth
    width: implicitWidth

    Switch {
        id: switchItem
        leftPadding: 0
    }
    CLabel {
        id: l
        Layout.preferredWidth: parent.width - switchItem.width - parent.spacing
        horizontalAlignment: Qt.AlignRight
    }
}
