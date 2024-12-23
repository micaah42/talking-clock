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
        anchors.rightMargin: 16
        anchors.leftMargin: 16
        spacing: 16

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
                Layout.fillWidth: true //Layout.preferredWidth: (index === 1 && false ? 2 : 1) * height
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
                        font.pixelSize: 88
                        text: model.icon()
                    }
                }
            }
        }
    }

    Card {
        visible: scale !== 0
        parent: window.contentItem
        anchors.fill: parent
        anchors.margins: 8

        MouseArea {
            anchors.fill: parent
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 8
            spacing: 16

            Card {
                Layout.preferredHeight: 56
                Layout.fillWidth: true
                bright: true

                CToolButton {
                    anchors.verticalCenter: parent.verticalCenter
                    text: Icons.chevron_backward
                    onClicked: currentPage = null
                    font.pixelSize: 32
                    height: 64
                    width: 64
                }

                CLabel {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    text: currentPage ? currentPage.name : ''
                    font.pixelSize: 24
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Loader {
                    id: loader
                    anchors.fill: parent
                    asynchronous: true

                    source: currentPage ? `qrc:/Clock/Pages/${currentPage.component}Page.qml` : ''
                    opacity: status === Loader.Ready ? 1 : 0

                    Behavior on opacity {
                        PropertyAnimation {}
                    }
                }

                BusyIndicator {
                    anchors.centerIn: parent
                    opacity: loader.status === Loader.Loading ? 1 : 0

                    Behavior on opacity {
                        PropertyAnimation {}
                    }
                }
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
