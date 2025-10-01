import QtQuick
import QtQuick.Layouts

import "../Style"
import "../Controls"

RowLayout {
    property string description
    property string title
    property string icon

    property alias descriptionItem: descriptionItem
    property alias titleItem: titleItem
    property alias iconItem: iconItem

    spacing: 24

    Icon {
        id: iconItem

        font.pixelSize: Theme.fontSizeXLarge
        visible: icon
        text: icon
    }

    ColumnLayout {
        spacing: 0

        CLabel {
            id: titleItem

            Layout.fillWidth: true
            elide: Text.ElideRight
            size: CLabel.Large
            visible: title
            text: title
        }

        CLabel {
            id: descriptionItem

            Layout.fillWidth: true
            elide: Text.ElideRight
            size: CLabel.Medium
            opacity: Theme.o72
            visible: description
            text: description
        }
    }
}
