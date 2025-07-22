import QtQuick
import QtQuick.Layouts 1.14
import QtQuick.Controls

import QtQml.Models

import Clock
import Clock.Pages
import Clock.Style
import Clock.Controls

Item {
    readonly property list<SectionNew> modeScreens: [
        SectionNew {
            content: GeneralSection {}
            title: 'General'
            icon: Icons.home
        },
        SectionNew {
            content: WirelessSection {}
            title: 'Wireless'
            icon: 'wifi'
        },
        SectionNew {
            content: AppearenceSection {}
            icon: Icons.display_settings
            title: 'Appearence'
        },
        SectionNew {
            content: PairedDevicesSection {}
            title: 'Paired Devices'
            icon: Icons.devices
        },
        SectionNew {
            content: SystemLightsSection {}
            title: 'System Lights'
            icon: Icons.model_training
        },
        SectionNew {
            content: PerformanceSection {}
            title: 'Performance'
            icon: Icons.monitoring
        },
        SectionNew {
            content: Item {
                property string title: 'About'
                property string icon: Icons.info
                AboutPage {}
            }
            icon: Icons.info
            title: 'About'
        }
    ]

    Component {
        id: sectionLoader
        FeedbackLoader {}
    }

    RowLayout {
        anchors.fill: parent
        spacing: 8
        clip: true

        ListView {
            id: navigationList
            Layout.preferredWidth: 200
            Layout.fillHeight: true

            interactive: contentHeight > height
            model: modeScreens

            onCurrentIndexChanged: {
                const properties = {
                    "sourceComponent": modeScreens[currentIndex].content
                }
                sections.replace(sectionLoader, properties)
            }

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
                onClicked: navigationList.currentIndex = index
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

        StackView {
            id: sections
            Layout.fillHeight: true
            Layout.fillWidth: true
            initialItem: Item {}
            clip: true
        }
    }
}
