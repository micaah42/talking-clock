import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts

ColumnLayout {
    id: root
    property alias labelText: l.text
    property alias label: l

    property alias spinBox: spinBox

    width: implicitWidth
    implicitWidth: 204

    CLabel {
        id: l
        opacity: 0.56
    }
    SpinBox {
        id: spinBox
        implicitWidth: root.width
        font.pixelSize: 24
    }
}
