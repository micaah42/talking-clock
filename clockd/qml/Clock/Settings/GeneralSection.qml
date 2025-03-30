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
}
