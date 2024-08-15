import QtQuick 2.14
import QtQuick.Controls 2.14

SpinBox {
    property alias labelText: l.text
    property alias label: l

    implicitWidth: 244
    font.pixelSize: 32

    CLabel {
        id: l

        anchors.bottom: parent.top
        font.pixelSize: 18
        background: Item {}
    }
}
