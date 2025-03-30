import QtQuick 2.14
import QtQuick.Controls
import QtQuick.Controls.Material

import Clock.Style

ToolButton {
    enum Size {
        Small,
        Medium,
        Large,
        XLarge,
        XXLarge
    }

    property int size: CToolButton.Medium
    font.family: Icons.fontFamily

    font.pixelSize: {
        switch (size) {
        case CToolButton.Small:
            return Theme.fontSizeSmall
        case CToolButton.Medium:
            return Theme.fontSizeMedium
        case CToolButton.Large:
            return Theme.fontSizeLarge
        case CToolButton.XLarge:
            return Theme.fontSizeXLarge
        case CToolButton.XXLarge:
            return Theme.fontSizeXXLarge
        }
    }
}
