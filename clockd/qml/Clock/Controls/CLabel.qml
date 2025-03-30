import QtQuick 2.0
import QtQuick.Controls 2.0

import Clock
import Clock.Style

Label {
    font.family: FontService.family
    elide: Text.ElideRight

    property int size: CLabel.Medium
    enum Size {
        Small,
        Medium,
        Large,
        XLarge,
        XXLarge
    }

    font.pixelSize: {
        switch (size) {
        case CLabel.Small:
            return Theme.fontSizeSmall
        case CLabel.Medium:
            return Theme.fontSizeMedium
        case CLabel.Large:
            return Theme.fontSizeLarge
        case CLabel.XLarge:
            return Theme.fontSizeXLarge
        case CLabel.XXLarge:
            return Theme.fontSizeXXLarge
        }
    }
}
