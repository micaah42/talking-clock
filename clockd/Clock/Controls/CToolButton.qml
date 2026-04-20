import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import "../Style"

ToolButton {
    id: root

    enum Size {
        Small,
        Medium,
        Large,
        XLarge,
        XXLarge
    }

    property int size: CToolButton.Medium
    flat: true

    contentItem: Icon {
        property int myPadding: 16
        rightPadding: myPadding
        leftPadding: myPadding
        bottomPadding: myPadding
        topPadding: myPadding
        padding: myPadding

        text: root.text
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

    background.data: Rectangle {
        anchors.fill: parent
        visible: !root.flat && root.enabled
        color: Theme.accent
        opacity: Theme.o72
        radius: width / 2
        z: -1
    }
}
