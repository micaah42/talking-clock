import QtQuick 
import QtQuick.Controls 
import QtQuick.Layouts

import Clock.Style

ColumnLayout {
    id: root
    property alias labelText: l.text
    property alias label: l

    property alias spinBox: spinBox

    implicitWidth: Theme.controlWidth
    width: implicitWidth

    CLabel {
        id: l
        opacity: Theme.o56
    }
    SpinBox {
        id: spinBox
        implicitWidth: root.width
        font.pixelSize: 24
    }
}
