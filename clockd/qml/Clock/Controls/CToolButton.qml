import QtQuick 2.14
import QtQuick.Controls
import QtQuick.Controls.Material

import Clock.Style

ToolButton {
    enum Size {
        Small,
        Medium,
        Large,
        XLarge
    }

    property int size: CToolButton.Medium
    font.family: Icons.fontFamily
    padding: -16

    implicitHeight: implicitWidth
    implicitWidth: {
        switch (size) {
        case CToolButton.Small:
            return 32
        case CToolButton.Medium:
            return 48
        case CToolButton.Large:
            return 64
        case CToolButton.XLarge:
            return 84
        }
    }

    font.pixelSize: {
        switch (size) {
        case CToolButton.Small:
            return 16
        case CToolButton.Medium:
            return 24
        case CToolButton.Large:
            return 32
        case CToolButton.XLarge:
            return 42
        }
    }
}
