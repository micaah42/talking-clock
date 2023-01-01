import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14
import QtQuick.VirtualKeyboard 2.15
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14
import "."


ApplicationWindow {
    id: window
    width: 840
    height: 480
    visible: true
    color: colorService.colors["background"]

    Material.theme: Material.Dark
    Material.accent: colorService.colors["accent"]
    Material.background: colorService.colors["background"]
    Material.primary: colorService.colors["primary"]

    SwipeView {
        // anchors.margins: 5
        anchors.fill: parent
        currentIndex: 1
        SettingsPage {}
        Clock {}
        AlarmPage {}
        TaskPage {}
    }

    InputPanel {
        id: keyboard
        anchors.fill: parent
        visible: Qt.inputMethod.visible
    }

    Connections {
        target: alarms
        function onAlarmTriggered(id) {
            var c = Qt.createComponent("MajorAlarm.qml")
            c.createObject(window, {alarm: alarms.model.at(id), width: window.width, height: window.height})
        }
    }

    Component.onCompleted: {
        console.log("view fully loaded :)");
        var c = Qt.createComponent("MajorAlarm.qml")
        c.createObject(window, {alarm: alarms.model.at(0), width: window.width, height: window.height})
    }

//    Button {
//        anchors.top: parent.top
//        anchors.right: parent.right
//        text: "bla"
//        onClicked: pop.popped = !pop.popped;
//    }

//    Frame {
//        id: pop
//        property bool popped: false

//        background: Rectangle { color: colorService.colors["accent"]; opacity: 0.5 }

//        height: popped ? parent.height / 4 : 0

//        anchors.margins: 5
//        anchors.bottom: parent.bottom
//        anchors.right: parent.right
//        anchors.left: parent.left

//        Behavior on height {
//            PropertyAnimation {
//                easing.type: Easing.InOutQuad
//                duration: 300
//            }
//        }
//    }
}
