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
        sourceComponent: Main {}
        active: true
    }

    ColumnLayout {
        anchors.centerIn: parent
        opacity: loader.status !== Loader.Ready
        visible: opacity
        spacing: 16

        Behavior on opacity {
            OpacityAnimator {}
        }

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
