import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts

ColumnLayout {
    property alias labelText: l.text
    property alias label: l

    property alias spinBox: spinBox

    implicitWidth: 244
    width: implicitWidth

    CLabel {
        id: l
        font.pixelSize: 14
    }
    SpinBox {
        id: spinBox
        implicitWidth: 244
        width: implicitWidth
        font.pixelSize: 24
        Component.onCompleted: console.log(height)
    }
}
