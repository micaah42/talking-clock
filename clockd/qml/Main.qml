import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14
import QtQuick.VirtualKeyboard 2.15
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14
import Qt.labs.platform 1.1

import Clock 1.0

ApplicationWindow {
    id: window
    width: 840
    height: 480
    visible: true
    color: colorService.colors["background"]

    Material.theme: Material.Dark
    Material.accent: colorService.colors["accent"]
    Material.primary: colorService.colors["primary"]
    Material.background: colorService.colors["background"]

    SwipeView {
        // anchors.margins: 5
        anchors.fill: parent
        currentIndex: 1
        SettingsPage {}
        Clock {}
        AlarmPage {}
    }

    InputPanel {
        id: keyboard
        anchors.fill: parent
        visible: Qt.inputMethod.visible
    }

    Connections {
        target: alarms
        function onAlarmTriggered(id) {
            var c = Qt.createComponent("AlarmPopup.qml")
            c.createObject(window, {
                               "alarm": alarms.model.at(id)
                           })
        }
    }

    Component.onCompleted: {
        console.log("view fully loaded :)")
        var popup = Qt.createComponent("AlarmPopup.qml")
        popup.createObject(window, {
                               "alarm": alarms.model.at(0)
                           })
    }
}
