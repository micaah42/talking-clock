import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14
import QtQuick.VirtualKeyboard
import QtMultimedia 5.15

import "alarms/"
import Clock 1.0
import Controls 1.0

Window {
    id: window

    width: 1024
    onWidthChanged: console.warn(`window.width=${width}`)

    height: 600
    onHeightChanged: console.warn(`window.height=${height}`)

    visible: true
    color: ColorService.background

    Material.theme: Material.Dark
    Material.accent: ColorService.accent
    Material.primary: ColorService.primary
    Material.background: ColorService.background
    Material.roundedScale: Material.NotRounded

    property real divisionY: Qt.inputMethod.visible ? height - keyboard.height : height

    Behavior on divisionY {
        PropertyAnimation {
            easing.type: Easing.InOutQuad
            duration: 250
        }
    }

    ScrollView {
        width: parent.width
        height: divisionY

        contentWidth: parent.width
        contentHeight: parent.height
        clip: true

        Stars {
            anchors.fill: parent
        }

        SwipeView {
            id: mainView

            height: parent.height
            width: parent.width
            currentIndex: 0

            Clock {
                MouseArea {
                    anchors.fill: parent
                    onClicked: mainView.currentIndex = (mainView.currentIndex + 1) % mainView.count
                }
            }

            MainMenu {}
        }
    }

    Loader {
        id: keyboard
        source: 'qrc:/Keyboard.qml'
    }

    Connections {
        target: EventFilter
        function onUserInactive() {
            mainView.currentIndex = 0
        }
    }

    Connections {
        target: AlarmService
        function onAlarmTriggered(id) {
            var comp = Qt.createComponent("alarms/AlarmNotification.qml", window)
            var alarm = AlarmService.model.at(id)
            var object = comp.createObject(window, {
                                               "alarm": alarm
                                           })
            object.dtor = object.destroy
        }
    }
}
