import QtQuick
import QtQuick.Layouts 1.14
import QtQuick.Controls

import Clock 1.0
import "../Controls"
import "../Style"

Section {
    id: objectModel

    title: 'Appearence'
    icon: Icons.display_settings

    CSpinBox {
        labelText: 'Star Count'
        spinBox.onValueModified: SpaceTheme.stars = spinBox.value
        spinBox.value: SpaceTheme.stars
        spinBox.stepSize: 25
        spinBox.from: 25
        spinBox.to: 500
    }
    CSpinBox {
        labelText: 'Animation Speed'
        spinBox.onValueModified: SpaceTheme.animationSpeed = spinBox.value
        spinBox.value: SpaceTheme.animationSpeed
        spinBox.stepSize: 250
        spinBox.from: 500
        spinBox.to: 10000
    }

    CTextField {
        placeholderText: 'Choose Font'
        text: FontService.family
        readOnly: true

        CFontDialog {
            id: fontDialog
            onFontFamilyAccepted: x => FontService.family = x
            fontFamily: FontService.family
        }

        MouseArea {
            anchors.fill: parent
            onClicked: fontDialog.open()
        }
    }

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
