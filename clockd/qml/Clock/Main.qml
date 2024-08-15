import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14
import QtQuick.VirtualKeyboard
import QtMultimedia 5.15

import Clock 1.0
import Clock.Main 1.0
import Clock.Controls 1.0

Window {
    id: window

    onWidthChanged: console.warn(`window.width=${width}`)
    width: 1920 / 2

    onHeightChanged: console.warn(`window.height=${height}`)
    height: 1080 / 2

    visible: true
    color: ColorService.background

    Material.theme: Material.Dark
    Material.accent: ColorService.accent
    Material.primary: ColorService.primary
    Material.background: ColorService.background
    Material.roundedScale: Material.NotRounded

    SpaceBackground {
        anchors.fill: parent
    }

    StackView {
        id: alarmStack

        height: parent.height
        width: 380

        initialItem: Item {}
        clip: true

        Component {
            id: alarmNotification
            AlarmNotification {}
        }

        Connections {
            target: AlarmService
            function onAlarmTriggered(alarm) {
                const properties = {
                    "alarm": alarm
                }

                alarmStack.push(alarmNotification, properties)
                mainView.currentIndex = 0
            }
        }
    }

    SwipeView {
        id: mainView
        anchors.right: parent.right

        width: alarmStack.depth <= 1 ? parent.width : parent.width - alarmStack.width + 64
        height: parent.height

        interactive: false
        currentIndex: 0
        clip: true

        Clock {
            MouseArea {
                anchors.fill: parent
                onClicked: mainView.currentIndex = 1
                enabled: alarmStack.depth < 2
            }
        }

        MainMenu {
            onBack: mainView.currentIndex = 0
        }
    }

    Loader {
        id: keyboard
        anchors.fill: parent
        source: 'qrc:/Clock/Controls/Keyboard.qml'
    }

    Connections {
        target: EventFilter
        function onUserInactive() {
            mainView.currentIndex = 0
        }
    }

    property var lastSwap: new Date()
    property real fps: 0

    onFrameSwapped: {
        const now = new Date()
        const diff = (now.getTime() - lastSwap.getTime())
        fps = 1000 / diff
        lastSwap = now
    }

    Label {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 8
        visible: SpaceTheme.fpsVisible
        text: fps.toFixed()
    }
}
