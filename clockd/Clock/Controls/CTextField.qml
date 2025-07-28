import QtQuick
import QtQuick.Controls

import Clock
import "../Style"

TextField {
    id: input

    font.family: FontService.family
    onAccepted: Qt.inputMethod.hide()

    font.pixelSize: Theme.fontSizeLarge
    implicitWidth: Theme.controlWidth
    width: implicitWidth
    topInset: 12
    padding: 8
}
