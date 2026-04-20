import QtQuick
import QtQuick.Layouts

import "../Style"
import "../Controls"

ColumnLayout {
    property bool expanded: false

    default property alias contentData: content.data
    property alias indicator: indicator
    property alias content: content

    property alias previewContent: preview.data
    property alias preview: preview

    property alias previewLabel: previewLabel
    spacing: 8

    RowLayout {
        id: mainContainer
        Layout.fillWidth: true
        spacing: 16

        RowLayout {
            id: preview
            Layout.fillWidth: true
            spacing: 8

            CLabel {
                id: previewLabel
                Layout.fillWidth: true
                visible: text
            }
        }

        CToolButton {
            id: indicator
            onClicked: expanded = !expanded
            text: Icons.chevron_left

            rotation: expanded ? -90 : 0
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
