import QtQuick
import QtQuick.Layouts

import Clock.Style
import Clock.Controls

ColumnLayout {
    property bool expanded: false
    property string title

    default property alias contentData: content.data
    property alias indicator: indicator
    property alias content: content
    property alias label: label

    property alias previewContent: previewContent
    property alias preview: preview
    spacing: 16

    RowLayout {
        id: preview
        Layout.fillWidth: true
        spacing: 24

        CLabel {
            id: label
            Layout.preferredWidth: 80
            visible: title
            text: title
        }

        Item {
            id: previewContent
            Layout.fillWidth: true
        }

        CToolButton {
            id: indicator

            text: Icons.keyboard_arrow_down
            onClicked: expanded = !expanded
            rotation: expanded ? 180 : 0

            Behavior on rotation {
                PropertyAnimation {}
            }
        }
    }

    ColumnLayout {
        id: content
        Layout.fillWidth: true
        visible: expanded
        spacing: 8
    }
}
