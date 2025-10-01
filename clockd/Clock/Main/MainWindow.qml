import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Clock
import "../Style"

Window {
    id: window

    onWidthChanged: console.warn(`window.width=${width}`)
    width: 1920 / 2

    onHeightChanged: console.warn(`window.height=${height}`)
    height: 1080 / 2

    Material.background: Theme.background
    Material.primary: Theme.primaryDark
    Material.accent: Theme.accentDark
    Material.theme: Material.Dark
    color: Theme.background
    visible: true

    Loader {
        id: loader
        anchors.fill: parent
        asynchronous: true
        sourceComponent: Main {}
        onLoaded: hide.start()
    }

    Rectangle {
        id: loadingScreen
        anchors.fill: parent
        color: Material.background

        SequentialAnimation on opacity {
            id: hide
            onFinished: loadingScreen.visible = false

            PauseAnimation {
                duration: 1500
            }
            OpacityAnimator {
                target: loadingScreen
                duration: 250
                from: 1
                to: 0
            }
        }

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 16

            Image {
                id: image
                Layout.alignment: Qt.AlignHCenter
                source: 'qrc:/icon-latest.png'
            }

            Item {
                id: rotators
                Layout.preferredHeight: 2 * image.height / 5
                Layout.preferredWidth: height
                Layout.alignment: Qt.AlignHCenter

                BusyIndicator {
                    id: indicator2
                    anchors.fill: rotators
                    anchors.margins: 8
                    Material.accent: '#f61f33'

                    transform: Scale {
                        origin.x: indicator2.width / 2
                        xScale: -1
                    }
                }

                BusyIndicator {
                    anchors.fill: rotators
                    Material.accent: '#4d92e5'
                }
            }
        }
    }
}
