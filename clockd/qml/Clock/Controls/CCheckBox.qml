import QtQuick 2.14
import QtQuick.Controls 2.14

import Clock
import Clock.Style

CheckBox {
    property alias labelText: l.text
    property alias label: l

    font.family: FontService.family

    CLabel {
        id: l
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.right
        opacity: Theme.o56
    }
}
