import QtQuick 2.14
import QtQuick.Controls 2.14
import Clock 1.0

CheckBox {
    property alias labelText: l.text
    property alias label: l

    font.family: FontService.family

    //implicitHeight: 56
    CLabel {
        id: l
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.right
        opacity: 0.56
    }
}
