import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

import QtQml.Models

import Clock
import Clock.Pages
import Clock.Style
import Clock.Controls

Item {
    RowLayout {
        anchors.fill: parent
        spacing: 8
        clip: true

        ListView {
            Layout.preferredWidth: 200
            Layout.fillHeight: true

            currentIndex: sections.currentIndex
            interactive: contentHeight > height
            model: sections.contentChildren

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
            opacity: Theme.o56
            radius: width / 2
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
            PairedDevicesSection {}
            SystemLightsSection {}
            PerformanceSection {}
            Item {
                property string title: 'About'
                property string icon: Icons.info
                AboutPage {}
            }
        }
    }
}
