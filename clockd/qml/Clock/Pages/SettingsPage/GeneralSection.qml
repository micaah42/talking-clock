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
    ConfirmButton {
        Layout.preferredWidth: textField.width
        onAccepted: System.powerOff()

        text: 'Power Off'
        dialogTitle: text
        dialogBodyText: 'Are you sure you want to power off the device?'
    }
    ConfirmButton {
        Layout.preferredWidth: textField.width
        onAccepted: System.reboot()

        text: 'Reboot'
        dialogTitle: text
        dialogBodyText: 'Are you sure you want to reboot the device?'
    }

    Item {
        Layout.fillHeight: true
        Layout.fillWidth: true
    }
}
