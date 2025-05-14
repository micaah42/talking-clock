import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

import Clock 1.0
import Clock.Style 1.0
import Clock.Controls 1.0

Section {
    title: 'General'
    icon: Icons.home

    CTextField {
        id: textField
        placeholderText: 'Device Name'
        text: 'Talking Clock'
    }

    CTextField {
        placeholderText: 'Timezone'
        readOnly: true

        MouseArea {
            anchors.fill: parent
            onClicked: timezoneDialog.open()
        }
    }

    Item {
        Layout.fillHeight: true
        Layout.fillWidth: true
    }
    ColumnLayout {
        spacing: -4
        ConfirmButton {
            Layout.preferredWidth: textField.width
            onAccepted: System.restartApplication()

            dialogBodyText: 'Are you sure you want restart the application?'
            dialogTitle: text
            text: 'Restart Application'
        }
        ConfirmButton {
            Layout.preferredWidth: textField.width
            onAccepted: System.powerOff()

            dialogBodyText: 'Are you sure you want to power off the device?'
            dialogTitle: text
            text: 'Power Off'
        }
        ConfirmButton {
            Layout.preferredWidth: textField.width
            onAccepted: System.reboot()
            dialogBodyText: 'Are you sure you want to reboot the device?'
            dialogTitle: text
            text: 'Reboot'
        }
    }

    Dialog {
        id: timezoneDialog

        title: 'Choose a Timezone'
        standardButtons: Dialog.Ok | Dialog.Cancel

        anchors.centerIn: Overlay.overlay
        height: window.contentItem.height - 64
        width: window.contentItem.width - 64

        contentItem: ColumnLayout {
            spacing: 0

            CTextField {
                Layout.fillWidth: true
                placeholderText: 'Search Timezone'
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true

                ListView {
                    anchors.fill: parent
                    anchors.topMargin: 8
                    model: TimeZoneModel

                    delegate: ItemDelegate {
                        id: d

                        property TimeZone timeZone: modelData
                        width: ListView.view.width

                        contentItem: ColumnLayout {
                            width: parent.width

                            Label {
                                text: d.timeZone.id + index.toString()
                            }
                            CLabel {
                                text: d.timeZone.displayName(new Date(), TimeZone.LongName)
                                size: CLabel.Small
                            }
                        }
                    }
                }
            }
        }
    }
}
