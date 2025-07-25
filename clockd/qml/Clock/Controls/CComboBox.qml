import QtQuick 
import QtQuick.Controls 

CTextField {
    id: ctrl
    property alias model: input.model
    property alias currentIndex: input.currentIndex
    signal activated(var index)

    onReleased: input.popup.open()
    onFocusChanged: focus = false
    readOnly: true

    ComboBox {
        id: input
        visible: false

        anchors.fill: parent
        background: Item {}
        displayText: ""

        popup.y: parent.height + 2
        popup.padding: 4

        popup.background: Rectangle {
            border.color: Material.CframeColor
            color: Material.dialogColor
            radius: 3
        }

        onActivated: {
            ctrl.text = model[currentIndex]
            ctrl.activated(currentIndex)
        }
    }
}
