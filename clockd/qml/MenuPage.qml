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

                                ColorOverlay {
                                    anchors.fill: parent
                                    antialiasing: true
                                    source: parent
                                    cached: true
                                    color: "#ffffff"
                                }
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
        anchors.fill: parent
        anchors.margins: 10

        ColumnLayout {
            anchors.fill: parent

            Card {
                Layout.preferredHeight: 50
                Layout.fillWidth: true
                Layout.margins: 8
                bright: true

                ToolButton {
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: close.start()
                    icon.source: 'qrc:/navigate_before_FILL0_wght700_GRAD200_opsz48.png'
                }

                Label {
                    id: title
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 24
                }
            }

            FeedbackLoader {
                id: loader
                Layout.fillWidth: true
                Layout.fillHeight: true

                onLoaded: open.start()
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
