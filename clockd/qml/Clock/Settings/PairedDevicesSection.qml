import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock
import Clock.Style
import Clock.Settings

Section {
    title: 'Paired Devices'
    icon: Icons.devices

    ListView {
        Layout.fillHeight: true
        Layout.fillWidth: true

        model: ClientService.clients

        delegate: ItemDelegate {
            property Client client: modelData
            width: ListView.view.width
            height: 60
            text: client.ip
        }
    }
}
