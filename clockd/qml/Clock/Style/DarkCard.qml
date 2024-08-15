import QtQuick 2.14

Rectangle {
    id: ctrl
    property alias enabled: mouse.enabled
    property bool checked: false

    signal clicked();


    color: colorService.colors["background"]
    radius: 5

    border.color: checked && enabled ? colorService.colors["primary"] : Qt.lighter(colorService.colors["darkprimary"], 0.8)
    border.width: checked && enabled ? 5 : 1.5

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
