import QtQuick 2.14
import QtQuick.Controls 2.14

CheckBox {
    property string label
    font.family: fontService.family

    CLabel {
        anchors.bottom: parent.top
        anchors.left: parent.left
        font.pixelSize: 12
        text: label
    }
}
