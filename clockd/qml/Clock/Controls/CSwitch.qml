import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    property alias labelText: l.text
    property alias label: l

    property alias switchItem: switchItem
    property alias checked: switchItem.checked

    width: implicitWidth
    implicitWidth: 244

    Switch {
        id: switchItem
        leftPadding: 0
    }
    Item {
        Layout.fillWidth: true
    }
    CLabel {
        id: l
        opacity: 0.56
    }
}
