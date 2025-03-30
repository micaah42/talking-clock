import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import Clock 1.0
import Clock.Style 1.0
import Clock.Controls 1.0
import Clock.Pages
import Clock.Alarms
import Clock.Settings

Item {
    property variant currentPage: null
    signal back

    onCurrentPageChanged: console.log('current page', currentPage)

    RowLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 16

        MainMenuButton {
            Layout.fillHeight: true
            Layout.fillWidth: true

            name: "Lighting"
            text: Icons.lightbulb_2
            comp: Component {
                LightingPage {}
            }

            property bool wasHeld: false
            Material.background: Lighting.enabled ? Theme.primary : Theme.primaryDark

            onPressAndHold: {
                Lighting.enabled = !Lighting.enabled
                wasHeld = true
            }

            onReleased: {
                if (!wasHeld)
                    currentPage = page
                wasHeld = false
            }
        }
        MainMenuButton {
            Layout.fillHeight: true
            Layout.fillWidth: true

            onClicked: currentPage = page

            name: "Alarm"
            text: Icons.alarm
            comp: Component {
                AlarmPage {}
            }
        }
        MainMenuButton {
            Layout.fillHeight: true
            Layout.fillWidth: true

            onClicked: currentPage = page

            name: "Settings"
            text: Icons.settings
            comp: Component {
                SettingsPage {}
            }
        }
    }
}
