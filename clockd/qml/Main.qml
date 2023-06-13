import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14
import QtQuick.VirtualKeyboard 2.15
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14
import Qt.labs.platform 1.1

import "alarms/"
import Clock 1.0
import Controls 1.0

ApplicationWindow {
    id: window
    width: 840
    height: 480
    visible: true
    color: ColorService.background

    Material.theme: Material.Dark
    Material.accent: ColorService.accent
    Material.primary: ColorService.primary
    Material.background: ColorService.background

    property real divisionY: Qt.inputMethod.visible ? height - keyboard.height : height

    Behavior on divisionY {
        PropertyAnimation {
            easing.type: Easing.InOutQuad
        }
    }

    ScrollView {
        width: parent.width
        height: divisionY

        contentWidth: window.width
        contentHeight: window.height

        clip: true

        Stars {
            anchors.fill: parent
        }

        SwipeView {
            id: mainView

            width: window.width
            height: window.height
            currentIndex: 0

            Clock {
                MouseArea {
                    anchors.fill: parent
                    onClicked: mainView.currentIndex = (mainView.currentIndex + 1) % mainView.count
                }
            }

            MenuPage {}
        }
    }

    InputPanel {
        id: keyboard
        width: parent.width
        y: divisionY
    }

    Connections {
        target: EventFilter
        function onUserInactive() {
            mainView.currentIndex = 0
        }
    }

    Connections {
        target: alarms
        function onAlarmTriggered(id) {
            var c = Qt.createComponent("alarms/AlarmPopup.qml")
            var alarm = alarms.model.at(id)
            c.createObject(window, {
                               "alarm": alarm
                           })
        }
    }
}
