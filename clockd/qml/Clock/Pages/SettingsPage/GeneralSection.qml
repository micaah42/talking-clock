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
        labelText: 'Device Name'
        text: 'Talking Clock'
    }
    CCheckBox {
        labelText: 'Special Days'
    }
    CCheckBox {
        labelText: 'Wheather'
    }
    Switch {
        Layout.fillWidth: true
        text: 'Am i aligned?'
    }

    Item {
        Layout.fillHeight: true
        Layout.fillWidth: true
    }
}
