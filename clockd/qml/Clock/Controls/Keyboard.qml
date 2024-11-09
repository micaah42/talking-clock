import QtQuick 2.14
import QtQuick.Controls
import QtQuick.VirtualKeyboard 2.15

Item {
    id: root

    Popup {
        id: popup
        visible: Qt.inputMethod.visible
        parent: window.contentItem
        height: keyboard.height
        width: parent.width
        background: Item {}
        padding: 0

        enter: Transition {
            PropertyAnimation {
                target: popup
                property: 'y'
                from: parent.height
                to: parent.height - keyboard.height
            }
        }
        exit: Transition {
            PropertyAnimation {
                target: popup
                property: 'y'
                to: parent.height
                from: parent.height - keyboard.height
            }
        }

        InputPanel {
            id: keyboard
            width: parent.width
        }
    }
}
