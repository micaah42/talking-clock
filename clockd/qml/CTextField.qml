import QtQuick 2.14
import QtQuick.Controls 2.14


TextField {
    id: input
    property alias label: l.text

    topPadding: 15
    implicitHeight: 55
    font.pixelSize: 20
    font.family: fontService.family

    CLabel {
        id: l
        x: 0; y: 0;
        font.pixelSize: 12
        background: Item {}
    }
}
