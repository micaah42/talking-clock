import QtQuick 

Rectangle {
    id: ctrl
    property alias enabled: mouse.enabled
    property bool checked: false

    signal clicked();


    color: Theme.colors["background"]
    radius: 5

    border.color: checked && enabled ? Theme.colors["primary"] : Qt.lighter(Theme.colors["darkprimary"], 0.8)
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
