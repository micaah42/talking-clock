import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

import QtQml.Models

import Clock 1.0
import Clock.Controls 1.0

Item {
    ListView {
        id: list
        anchors.fill: parent
        anchors.margins: 15
        model: objectModel
        spacing: 32
    }

    ObjectModel {
        id: objectModel

        Label {
            text: 'Space Background Settings'
            font.pixelSize: 24
        }
        CSpinBox {
            labelText: 'Star Count'
            onValueModified: SpaceTheme.stars = value
            value: SpaceTheme.stars
            stepSize: 5
            from: 25
            to: 100
        }
        CSpinBox {
            labelText: 'Animated Star Count'
            onValueModified: SpaceTheme.animatedStars = value
            value: SpaceTheme.animatedStars
            stepSize: 5
            from: 25
            to: 75
        }
        CSpinBox {
            labelText: 'Animation Speed'
            onValueModified: SpaceTheme.animationSpeed = value
            value: SpaceTheme.animationSpeed
            stepSize: 250
            from: 500
            to: 10000
        }
        CCheckBox {
            labelText: 'Space Ship'
            onCheckedChanged: SpaceTheme.spaceShip = checked
            checked: SpaceTheme.spaceShip
        }
        CCheckBox {
            labelText: 'FPS Counter Visible'
            onCheckedChanged: SpaceTheme.fpsVisible = checked
            checked: SpaceTheme.fpsVisible
        }
    }
}
