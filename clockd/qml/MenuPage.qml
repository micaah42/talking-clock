import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtGraphicalEffects 1.15

import Clock 1.0

Item {
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
                    icon: "qrc:/neurology_FILL0_wght400_GRAD0_opsz48.svg"
                    name: "Brains"
                }
                ListElement {
                    icon: "qrc:/work_FILL0_wght400_GRAD0_opsz48.svg"
                    name: "Jobs"
                }
                ListElement {
                    component: "qrc:/settings/SettingsPage.qml"
                    icon: "qrc:/settings_FILL0_wght400_GRAD0_opsz48.svg"
                    name: "Settings"
                }
                ListElement {
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
                    onClicked: {
                        loader.source = model.component
                        title.text = model.name
                    }

                    contentItem: Item {
                        anchors.centerIn: parent

                        ColumnLayout {
                            anchors.centerIn: parent
                            width: 0.6 * parent.height

                            Image {
                                id: icon
                                Layout.fillWidth: true
                                Layout.preferredHeight: width
                                sourceSize.height: height
                                sourceSize.width: width
                                Layout.alignment: Qt.AlignHCenter
                                source: model.icon

                                ColorOverlay {
                                    anchors.fill: parent
                                    antialiasing: true
                                    source: parent
                                    color: "#ffffff"
                                }
                            }
                            Label {
                                Layout.alignment: Qt.AlignHCenter
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
        anchors.fill: parent
        anchors.margins: 10

        ColumnLayout {
            anchors.fill: parent

            Card {
                visible: loader.status === Loader.Ready
                color: ColorService.primary
                Layout.preferredHeight: 50
                Layout.fillWidth: true
                Layout.margins: 5

                ToolButton {
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: close.start()
                    text: '<'
                }

                Label {
                    id: title
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 24
                }
            }

            Loader {
                id: loader
                Layout.fillWidth: true
                Layout.fillHeight: true

                onStatusChanged: {
                    if (status === Loader.Ready) {
                        open.start()
                    }
                }
            }
        }

        PropertyAnimation on scale {
            id: close
            easing.type: Easing.InOutQuad
            onFinished: loader.source = ''
            duration: 250
            to: 0
        }

        Connections {
            target: EventFilter
            function onUserInactive() {
                close.start()
            }
        }

        PropertyAnimation on scale {
            id: open
            easing.type: Easing.InOutQuad
            duration: 250
            to: 1
        }
    }
}
