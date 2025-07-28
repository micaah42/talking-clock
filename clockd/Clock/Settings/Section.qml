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
    spacing: 16

    RowLayout {
        Layout.bottomMargin: 16
        spacing: 16

        Icon {
            text: icon
        }

        CLabel {
            Layout.fillWidth: true
            font.weight: Font.DemiBold
            font.pixelSize: 28
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
