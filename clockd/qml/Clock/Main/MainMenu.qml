import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import Clock 1.0
import Clock.Style 1.0
import Clock.Controls 1.0

Item {
    property variant currentPage: null
    signal back

    onCurrentPageChanged: console.log('current page', currentPage)

    RowLayout {
        anchors.fill: parent
        anchors.rightMargin: 4
        anchors.leftMargin: 4
        spacing: 16

        Item {
            Layout.fillWidth: true
        }

        Repeater {
            model: ListModel {
                ListElement {
                    component: "Lighting"
                    icon: () => Icons.lightbulb_2
                    //icon: "qrc:/calendar_month_24dp_E8EAED_FILL0_wght400_GRAD0_opsz48.svg"
                    name: "Lighting"
                }
                ListElement {
                    component: "Alarm"
                    icon: () => Icons.alarm
                    name: "Alarms"
                }
                ListElement {
                    component: "Settings"
                    icon: () => Icons.settings
                    name: "Settings"
                }
            }

            delegate: Button {
                Layout.preferredWidth: (index === 1 ? 2 : 1) * height
                Layout.fillHeight: true

                Material.roundedScale: Material.SmallScale
                Material.background: ColorService.primary
                enabled: model.component.length

                onClicked: {
                    currentPage = model
                    console.log(currentPage, model)
                }

                contentItem: Item {
                    Icon {
                        id: icon

                        anchors.centerIn: parent
                        font.weight: Font.Thin
                        font.pixelSize: 88
                        text: model.icon()
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: true
        }
    }

    Card {
        visible: scale !== 0
        parent: window.contentItem
        anchors.fill: parent

        //anchors.margins: 16
        MouseArea {
            anchors.fill: parent
        }

        ColumnLayout {
            anchors.fill: parent

            Card {
                Layout.preferredHeight: 56
                Layout.fillWidth: true
                Layout.margins: 8
                bright: true

                CToolButton {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 8
                    text: Icons.chevron_backward
                    onClicked: currentPage = null
                    font.pixelSize: 24
                }

                CLabel {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    text: currentPage ? currentPage.name : ''
                    font.pixelSize: 24
                }
            }

            Loader {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.margins: 8
                Layout.rightMargin: 24
                Layout.leftMargin: 24
                source: currentPage ? `qrc:/Clock/Pages/${currentPage.component}Page.qml` : ''
                visible: status === Loader.Ready
                asynchronous: true
            }
        }

        scale: currentPage ? 1 : 0

        Behavior on scale {
            PropertyAnimation {
                easing.type: Easing.InOutQuad
                duration: 250
            }
        }

        Connections {
            target: EventFilter
            function onUserInactive() {
                currentPage = null
            }
        }
    }
}
