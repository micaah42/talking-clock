import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import Clock 1.0

Item {
    property variant currentPage: null
    onCurrentPageChanged: console.log('current page', currentPage)

    GridLayout {
        anchors.fill: parent
        columns: 3
        rows: 2

        Repeater {
            model: ListModel {
                ListElement {
                    component: "qrc:/alarms/AlarmPage.qml"
                    icon: "qrc:/alarm_FILL0_wght400_GRAD0_opsz48.svg"
                    name: "Alarms"
                }
                ListElement {
                    component: "qrc:/data/DataPage.qml"
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
                    component: "qrc:/settings/SettingsPage.qml"
                    icon: "qrc:/settings_FILL0_wght400_GRAD0_opsz48.svg"
                    name: "Settings"
                }
                ListElement {
                    component: "qrc:/Pages/AboutPage.qml"
                    icon: "qrc:/info_FILL0_wght400_GRAD0_opsz48.svg"
                    name: "About"
                }
            }

            delegate: Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Button {
                    anchors.centerIn: parent
                    width: 0.75 * parent.width
                    height: 0.75 * parent.height
                    Material.background: ColorService.darkPrimary
                    Material.roundedScale: Material.ExtraSmallScale
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
    }

    Card {
        visible: scale !== 0
        anchors.fill: parent
        anchors.margins: 10

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
                source: currentPage ? currentPage.component : ''
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
