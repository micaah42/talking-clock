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
    CSpinBox {
        labelText: 'Star Count'
        spinBox.onValueModified: SpaceTheme.stars = spinBox.value
        spinBox.value: SpaceTheme.stars
        spinBox.stepSize: 5
        spinBox.from: 5
        spinBox.to: 100
    }
    CSpinBox {
        labelText: 'Animated Star Count'
        spinBox.onValueModified: SpaceTheme.animatedStars = spinBox.value
        spinBox.value: SpaceTheme.animatedStars
        spinBox.stepSize: 5
        spinBox.from: 5
        spinBox.to: 75
    }
    CSpinBox {
        labelText: 'Animation Speed'
        spinBox.onValueModified: SpaceTheme.animationSpeed = spinBox.value
        spinBox.value: SpaceTheme.animationSpeed
        spinBox.stepSize: 250
        spinBox.from: 500
        spinBox.to: 10000
    }
    CCheckBox {
        labelText: 'FPS Counter Visible'
        onCheckedChanged: SpaceTheme.fpsVisible = checked
        checked: SpaceTheme.fpsVisible
    }
    CCheckBox {
        labelText: 'Space Ship'
        onCheckedChanged: SpaceTheme.spaceShip = checked
        checked: SpaceTheme.spaceShip
    }

    Item {
        Layout.fillHeight: true
        Layout.fillWidth: true
    }
}