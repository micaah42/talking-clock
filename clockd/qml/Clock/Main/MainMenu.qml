import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import Clock 1.0
import Clock.Style 1.0

Item {
    property variant currentPage: null
    signal back

    onCurrentPageChanged: console.log('current page', currentPage)

    GridLayout {
        anchors.fill: parent
        anchors.margins: 24

        columnSpacing: 32
        columns: 3

        rowSpacing: 20
        rows: 3

        Button {
            Material.background: ColorService.primary
            Material.roundedScale: Material.SmallScale
            Layout.preferredHeight: 64
            Layout.bottomMargin: -8
            Layout.fillWidth: true
            Layout.columnSpan: 3

            font.pixelSize: 24
            onClicked: back()
            text: 'Main Menu'

            ToolButton {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                icon.source: 'qrc:/navigate_before_FILL0_wght400_GRAD0_opsz24.svg'
                onClicked: back()
            }
        }

        Repeater {
            model: ListModel {
                ListElement {
                    component: "Alarm"
                    icon: "qrc:/alarm_FILL0_wght400_GRAD0_opsz48.svg"
                    name: "Alarms"
                }
                ListElement {
                    component: ""
                    icon: "qrc:/database_FILL0_wght400_GRAD0_opsz48.svg"
                    name: "Data"
                }
                ListElement {
                    component: ""
                    icon: "qrc:/neurology_FILL0_wght400_GRAD0_opsz48.svg"
                    name: "Brains"
                }
                ListElement {
                    component: ""
                    icon: "qrc:/work_FILL0_wght400_GRAD0_opsz48.svg"
                    name: "Jobs"
                }
                ListElement {
                    component: "Settings"
                    icon: "qrc:/settings_FILL0_wght400_GRAD0_opsz48.svg"
                    name: "Settings"
                }
                ListElement {
                    component: "About"
                    icon: "qrc:/info_FILL0_wght400_GRAD0_opsz48.svg"
                    name: "About"
                }
            }

            delegate: Button {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Material.roundedScale: Material.SmallScale
                Material.background: ColorService.darkPrimary
                enabled: model.component.length

                onClicked: {
                    currentPage = model
                    console.log(currentPage, model)
                }

                contentItem: Item {
                    Column {
                        anchors.centerIn: parent
                        width: 128

                        Image {
                            id: icon
                            source: model.icon

                            width: parent.width
                            height: width

                            fillMode: Image.PreserveAspectFit
                            sourceSize.width: width
                            sourceSize.height: height
                        }

                        Label {
                            horizontalAlignment: Qt.AlignHCenter
                            width: parent.width
                            font.pixelSize: 24
                            text: model.name
                        }
                    }
                }
            }
        }
    }

    Card {
        visible: scale !== 0
        anchors.fill: parent
        anchors.margins: 16

        MouseArea {
            anchors.fill: parent
        }

        ColumnLayout {
            anchors.fill: parent

            Card {
                Layout.preferredHeight: 50
                Layout.fillWidth: true
                Layout.margins: 8
                bright: true

                ToolButton {
                    anchors.verticalCenter: parent.verticalCenter
                    icon.source: 'qrc:/navigate_before_FILL0_wght400_GRAD0_opsz24.svg'
                    onClicked: currentPage = null
                }

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    text: currentPage ? currentPage.name : ''
                    font.pixelSize: 24
                }
            }

            Loader {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 8
                source: currentPage ? `qrc:/Clock/Pages/${currentPage.component}Page.qml` : ''
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
