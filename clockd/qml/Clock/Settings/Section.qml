import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14

import Clock.Style
import Clock.Controls

ColumnLayout {
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
}
