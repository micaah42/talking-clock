import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock 1.0
import Clock.Style 1.0
import Clock.Controls 1.0

Section {
    id: performanceSection
    title: 'Performance'
    icon: Icons.monitoring
    property bool isCurrentItem: SwipeView.isCurrentItem

    Component.onDestruction: CPUMonitor.unsubscribe(this)
    Component.onCompleted: CPUMonitor.subscribe(this)

    RowLayout {
        Layout.fillWidth: true

        CLabel {
            text: 'CPU Usage'
        }
        Item {
            Layout.fillWidth: true
        }
        CLabel {
            readonly property var infos: [CPUMonitor.model, CPUMonitor.vendor, CPUMonitor.architecture]
            text: infos.filter(x => x.length).join(' \u2022 ')
        }
    }

    Item {
        Layout.preferredHeight: parent.height / 3
        Layout.fillWidth: true

        id: item

        Repeater {
            model: 5
            delegate: Rectangle {
                y: modelData * item.height / 4
                opacity: Theme.o42
                width: item.width
                height: 0.75
            }
        }

        CLabel {
            text: '100%'
            y: 2
        }
        CLabel {
            anchors.bottom: parent.bottom
            text: graph.duration / 1000 + 's'
        }

        CPUGraph {
            id: graph
            anchors.fill: parent
            colors: Palettes.palettes[2]["colors"]
            cpu: CPUMonitor
        }
    }

    GridLayout {
        Layout.fillWidth: true
        columns: 8
        Repeater {
            model: Array.from(CPUMonitor.cores).slice(1)
            delegate: Card {
                Layout.preferredHeight: 24
                Layout.fillWidth: true
                bright: true
                CLabel {
                    anchors.centerIn: parent
                    font.pixelSize: 18
                    text: `${index + 1}: ${modelData.load.toFixed(0)}%`
                }
            }
        }
    }

    ValueDisplay {
        Layout.preferredWidth: 120
        Layout.topMargin: 16
        labelText: 'CPU Temp'
        valueText: CPUMonitor.temperature
    }
    Item {
        Layout.fillHeight: true
    }
}
