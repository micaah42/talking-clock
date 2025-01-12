import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import Clock 1.0
import Clock.Style 1.0
import Clock.Controls 1.0
import Clock.Pages

Item {
    property variant currentPage: null
    signal back

    onCurrentPageChanged: console.log('current page', currentPage)

    RowLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 16

        Button {
            Material.roundedScale: Material.SmallScale
            Layout.fillHeight: true
            Layout.fillWidth: true
            implicitWidth: 0

            property bool wasHeld: false
            Material.background: Lighting.enabled ? Qt.lighter(ColorService.primary, 1.75) : ColorService.primary

            onPressAndHold: {
                Lighting.enabled = !Lighting.enabled
                wasHeld = true
            }

            onReleased: {
                if (!wasHeld)
                    currentPage = page
                wasHeld = false
            }
            readonly property var page: {
                "component": "Lighting",
                "icon": Icons.lightbulb_2,
                "name": "Lighting"
            }

            font.family: Icons.fontFamily
            font.pixelSize: 88
            text: page.icon
        }
        Button {
            Material.roundedScale: Material.SmallScale
            Layout.fillHeight: true
            Layout.fillWidth: true
            implicitWidth: 0

            Material.background: ColorService.primary
            onClicked: currentPage = page
            readonly property var page: {
                "component": "Alarm",
                "icon": Icons.alarm,
                "name": "Alarm"
            }

            font.family: Icons.fontFamily
            font.pixelSize: 88
            text: page.icon
        }
        Button {
            Material.roundedScale: Material.SmallScale
            Material.background: ColorService.primary

            Layout.fillHeight: true
            Layout.fillWidth: true
            implicitWidth: 0
            onClicked: currentPage = page
            readonly property var page: {
                "component": "Settings",
                "icon": Icons.settings,
                "name": "Settings"
            }

            font.family: Icons.fontFamily
            font.pixelSize: 88
            text: page.icon
        }
    }
}
