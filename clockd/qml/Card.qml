import QtQuick 2.0

Rectangle {
    id: ctrl
    property alias enabled: mouse.enabled
    property bool checked: false

    signal clicked();


    color: mouse.containsMouse | mouse.pressed ? Qt.lighter(colorService.colors["darkprimary"], 1.25)
                                               : colorService.colors["darkprimary"]
    radius: 5

    border.color: checked && enabled ? colorService.colors["primary"] : Qt.lighter(colorService.colors["darkprimary"], 0.8)
    border.width: checked && enabled ? 2 : 1

    MouseArea {
        id: mouse
        enabled: false
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            // ctrl.checked = !ctrl.checked;
            ctrl.clicked();
        }
    }
}
