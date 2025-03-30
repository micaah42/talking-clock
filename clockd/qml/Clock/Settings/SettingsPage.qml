import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

import QtQml.Models

import Clock
import Clock.Style
import Clock.Controls
import Clock.Settings
import Clock.Pages

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

            WirelessSection {}
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
