import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14
import QtQuick.VirtualKeyboard
import QtMultimedia 5.15

import Clock 1.0
import Clock.Main 1.0
import Clock.Pages.AlarmPage
import Clock.Controls 1.0
import Clock.Style 1.0

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

    SpaceScene {
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
                mainMenu.currentPage = null
                drawer.open = false
            }
        }
    }

    Clock {
        anchors.right: parent.right

        width: alarmStack.depth <= 1 ? parent.width : parent.width - alarmStack.width + 64
        Behavior on width {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
            }
        }

        height: !drawer.open ? parent.height : parent.height - drawer.height
        Behavior on height {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
            }
        }

        scale: drawer.open ? 0.6 : 1
        Behavior on scale {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: drawer.open = !drawer.open
        enabled: alarmStack.depth < 2
    }

    Item {
        id: drawer

        property bool open: false
        width: parent.width
        height: 200

        anchors.bottom: parent.bottom
        anchors.bottomMargin: open ? 0 : -height

        Behavior on anchors.bottomMargin {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
            }
        }

        Card {
            anchors.fill: parent
            anchors.margins: 8
            //backgroundOpacity: 0.9

            //bright: true
            MainMenu {
                id: mainMenu
                onCurrentPageChanged: drawer.open = false
                anchors.fill: parent
                anchors.margins: 8
            }
        }
    }

    Item {
        x: drawer.open ? 0 : -width
        Behavior on x {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
            }
        }

        height: parent.height - drawer.height
        width: 280

        Card {
            anchors.fill: parent
            anchors.margins: 8
            anchors.leftMargin: 10
            //backgroundOpacity: 0.9

            //bright: true
            NextAlarm {
                anchors.fill: parent
                anchors.margins: 16
                alarm: AlarmService.nextAlarm
            }
        }
    }

    Item {
        x: drawer.open ? parent.width - width : parent.width
        Behavior on x {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
            }
        }

        height: parent.height - drawer.height
        width: 280

        Card {
            anchors.fill: parent
            anchors.margins: 8
            anchors.rightMargin: 10
            //backgroundOpacity: 0.9

            //bright: true
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16

                SwipeView {
                    id: view
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    onCurrentIndexChanged: timer.restart()
                    clip: true

                    Repeater {
                        model: ActionDayManager.days

                        delegate: ColumnLayout {
                           CLabel {
                                Layout.fillWidth: true
                                text: modelData.name
                                wrapMode: Text.Wrap
                                font.pixelSize: 32
                            }
                           CLabel {
                                Layout.fillWidth: true
                                text: modelData.desc
                                wrapMode: Text.Wrap
                                font.pixelSize: 18
                            }
                            Item {
                                Layout.fillHeight: true
                            }
                           CLabel {
                                font.underline: true
                                font.pixelSize: 18
                                text: 'Link'
                            }
                        }
                    }

                    Timer {
                        id: timer
                        onTriggered: view.currentIndex = (view.currentIndex + 1) % view.count
                        interval: 7500
                        running: true
                    }
                }

                PageIndicator {
                    Layout.alignment: Qt.AlignHCenter
                    currentIndex: view.currentIndex
                    count: view.count
                }
            }
        }
    }

    Loader {
        id: keyboard
        anchors.fill: parent
        source: 'qrc:/Clock/Controls/Keyboard.qml'
        z: 1
    }

    Connections {
        target: EventFilter
        function onUserInactive() {
            mainMenu.currentPage = null
            drawer.open = false
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

   CLabel {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 8
        visible: SpaceTheme.fpsVisible
        font.pixelSize: 24
        text: fps.toFixed()
    }
}
