import QtQuick 2.14
import QtQuick.Controls 2.14

CTextField {
    id: ctrl
    property alias model: input.model
    property alias currentIndex: input.currentIndex
    signal activated(var index)
    onReleased: input.popup.open();
    onFocusChanged: focus = false;

    ComboBox {
        id: input
        anchors.fill: parent
        visible: false
        displayText: ""
        width: parent.width
        background: Item {}
        onActivated: {
            ctrl.activated(index);
            ctrl.text = model[index];
        }
    }
}
