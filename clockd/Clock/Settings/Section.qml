import QtQuick
import QtQuick.Layouts 1.14
import QtQuick.Controls

import "../Style"
import "../Controls"

ColumnLayout {
    default property alias content: contents.data
    property Component extraHeaderContent
    property string title
    property string icon
    spacing: 24

    RowLayout {
        Layout.bottomMargin: 8
        spacing: 16

        Icon {
            font.pixelSize: Theme.fontSizeXLarge
            text: icon
        }
        CLabel {
            Layout.fillWidth: true
            font.weight: Font.DemiBold
            size: CLabel.Large
            text: title
        }
        Loader {
            sourceComponent: extraHeaderContent
        }
    }

    ColumnLayout {
        id: contents
        Layout.fillWidth: true
        spacing: 8
    }
}
