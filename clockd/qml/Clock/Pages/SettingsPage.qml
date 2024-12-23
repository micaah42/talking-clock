import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

import QtQml.Models

import Clock 1.0
import Clock.Style 1.0
import Clock.Controls 1.0
import Clock.Pages.SettingsPage 1.0

Item {
    RowLayout {
        anchors.fill: parent
        spacing: 8
        clip: true

        ListView {
            Layout.preferredWidth: 200
            Layout.fillHeight: true

            model: sections.contentChildren
            currentIndex: sections.currentIndex
            interactive: false

            delegate: ItemDelegate {
                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16
                    spacing: 16

                    Icon {
                        text: modelData.icon
                    }
                    CLabel {
                        text: modelData.title
                        font.pixelSize: 24
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                }

                highlighted: ListView.isCurrentItem
                onClicked: sections.currentIndex = index
                width: parent.width
                height: 56
            }
        }

        Rectangle {
            Layout.preferredWidth: 1.5
            Layout.fillHeight: true
            radius: width / 2
            opacity: 0.5
        }

        SwipeView {
            id: sections
            orientation: Qt.Vertical
            Layout.fillHeight: true
            Layout.fillWidth: true
            interactive: false
            spacing: 8
            GeneralSection {}

            Section {
                id: performanceSection
                title: 'Performance'
                icon: Icons.monitoring
                property bool isCurrentItem: SwipeView.isCurrentItem

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

                Loader {
                    Layout.preferredHeight: parent.height / 3
                    Layout.fillWidth: true
                    active: performanceSection.isCurrentItem

                    sourceComponent: Item {
                        id: item

                        Repeater {
                            model: 5
                            delegate: Rectangle {
                                opacity: 0.32
                                y: modelData * item.height / 4
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

                RowLayout {
                    Layout.fillHeight: true
                    Layout.topMargin: 16

                    ValueDisplay {
                        Layout.preferredWidth: 120
                        labelText: 'FPS'
                        valueText: window.fps.toFixed(0)
                    }
                    ValueDisplay {
                        Layout.preferredWidth: 120
                        labelText: 'CPU Temp'
                        valueText: CPUMonitor.temperature
                    }
                }
                Item {
                    Layout.fillHeight: true
                }
            }

            AppearenceSection {}

            Section {
                title: 'Paired Devices'
                icon: Icons.devices

                ListView {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    model: ClientService.clients

                    delegate: ItemDelegate {
                        property Client client: modelData
                        width: ListView.view.width
                        height: 60
                        text: client.ip
                    }
                }
            }

            Item {
                property string title: 'About'
                property string icon: Icons.info
                AboutPage {}
            }
        }
    }
}
