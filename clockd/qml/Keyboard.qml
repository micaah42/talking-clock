import QtQuick 2.14
import QtQuick.Controls
import QtQuick.VirtualKeyboard 2.15

Item {
    id: root
    height: keyboard.height

    Popup {
        visible: Qt.inputMethod.visible

        parent: window.contentItem
        height: keyboard.height
        width: window.width
        y: divisionY

        onClosed: Qt.inputMethod.hide()
        padding: 0

        InputPanel {
            id: keyboard
            width: window.w
        }
    }
}
