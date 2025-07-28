import QtQuick
import QtQuick.Controls
import QPerformance

import Clock
import "../Style"
import "../Controls"

Item {
    id: root

    property real smoothFPS: 0
    property real fps: 0

    property var lastSwap: new Date()
    property int swapCount: 0

    Connections {
        target: window
        enabled: SpaceTheme.fpsVisible
        function onFrameSwapped() {
            const now = new Date()
            const diff = (now.getTime() - lastSwap.getTime())

            fps = 1000 / diff
            lastSwap = now
            chart.pushValues([fps])
            swapCount++
        }
    }

    Timer {
        interval: 1000
        running: true
        repeat: true

        onTriggered: {
            smoothFPS = swapCount
            swapCount = 0
        }
    }

    Popup {
        id: popup
        visible: SpaceTheme.fpsVisible
        height: root.height
        width: root.width

        closePolicy: Popup.NoAutoClose
        padding: 4

        background: CFrame {
            backgroundColor: Theme.alpha(Material.frameColor, Theme.o24)
        }

        contentItem: Item {
            PerformanceChart {
                id: chart
                anchors.fill: parent
                anchors.margins: -popup.padding
                colors: Theme.accent
                duration: 2000
            }
            CLabel {
                anchors.right: parent.right
                anchors.top: parent.top
                font.pixelSize: 24
                text: smoothFPS.toFixed()
            }
        }
    }
}
