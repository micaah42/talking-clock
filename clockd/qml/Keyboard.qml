import QtQuick 2.14
import QtQuick.Controls
import QtQuick.VirtualKeyboard 2.15

Item {
    id: root
    height: keyboard.height

    Popup {
        visible: Qt.inputMethod.visible

        parent: window.contentItem

        scale: window.s === 0 ? 1 : 1 / window.s
        height: keyboard.height
        width: window.w
        y: divisionY

        onClosed: Qt.inputMethod.hide()
        padding: 0

        InputPanel {
            id: keyboard
            width: window.w
        }
    }
}
