import QtQuick 2.14
import QtQuick.Controls
import QtQuick.VirtualKeyboard 2.15

Item {
    id: root

    InputPanel {
        id: keyboard

        visible: Qt.inputMethod.visible
        anchors.bottom: parent.bottom
        width: parent.width
    }
}
