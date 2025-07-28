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
    spacing: 16

    RowLayout {
        id: mainContainer
        Layout.fillWidth: true
        spacing: 16

        RowLayout {
            id: preview
            Layout.fillWidth: true
            spacing: 8
        }

        CToolButton {
            id: indicator
            onClicked: expanded = !expanded
            text: Icons.chevron_left

            rotation: expanded ? 90 : -90
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
