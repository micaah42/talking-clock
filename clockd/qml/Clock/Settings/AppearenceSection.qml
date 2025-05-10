import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

import Clock 1.0
import Clock.Controls 1.0
import Clock.Style 1.0

Section {
    id: objectModel

    title: 'Appearence'
    icon: Icons.display_settings
    RowLayout {
        spacing: 16

        ColumnLayout {
            Layout.maximumWidth: 204
            spacing: 16
            CSpinBox {
                labelText: 'Star Count'
                spinBox.onValueModified: SpaceTheme.stars = spinBox.value
                spinBox.value: SpaceTheme.stars
                spinBox.stepSize: 10
                spinBox.from: 10
                spinBox.to: 250
            }
            CSpinBox {
                labelText: 'Animation Speed'
                spinBox.onValueModified: SpaceTheme.animationSpeed = spinBox.value
                spinBox.value: SpaceTheme.animationSpeed
                spinBox.stepSize: 250
                spinBox.from: 500
                spinBox.to: 10000
            }
            CFontDialog {
                Layout.preferredHeight: 56
                Layout.fillWidth: true
                onFontFamilyAccepted: x => FontService.family = x
                fontFamily: FontService.family
                label: 'Choose Font'
            }
            Item {
                Layout.fillHeight: true
            }
        }
        ColumnLayout {
            Layout.maximumWidth: 204
            spacing: 16
            CSwitch {
                labelText: 'FPS Counter Visible'
                onCheckedChanged: SpaceTheme.fpsVisible = checked
                checked: SpaceTheme.fpsVisible
            }
            CSwitch {
                labelText: 'Space Ship'
                onCheckedChanged: SpaceTheme.spaceShip = checked
                checked: SpaceTheme.spaceShip
            }
            Item {
                Layout.fillHeight: true
            }
        }
        Item {
            Layout.fillWidth: true
        }
    }
}
