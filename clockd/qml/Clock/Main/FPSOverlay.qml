import QtQuick
import QtQuick.Controls
import QPerformance

import Clock
import Clock.Style
import Clock.Controls

Item {
    id: root

    property var lastSwap: new Date()
    property real fps: 0

    Component.onCompleted: SpaceTheme.fpsVisible = true
    Connections {
        target: window
        enabled: SpaceTheme.fpsVisible
        function onFrameSwapped() {
            const now = new Date()
            const diff = (now.getTime() - lastSwap.getTime())
            fps = 1000 / diff
            lastSwap = now
            chart.newValues([fps], now.getTime())
        }
    }

    Popup {
        id: popup
        visible: SpaceTheme.fpsVisible
        height: root.height
        width: root.width

        closePolicy: Popup.NoAutoClose
        background: CFrame {
            backgroundColor: Theme.alpha(Material.foreground, Theme.o11)
        }

        contentItem: Item {
            CLabel {
                anchors.right: parent.right
                anchors.top: parent.top
                font.pixelSize: 24
                text: fps.toFixed()
            }
            LongTimeChart {
                id: chart
                anchors.fill: parent
                anchors.margins: -popup.padding
                colors: Theme.accent
                duration: 2500
            }
        }
    }
}
