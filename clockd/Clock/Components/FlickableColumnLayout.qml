import QtQuick
import QtQuick.Layouts

Flickable {
    id: root

    default property alias columnLayoutData: columnLayout.data
    property alias columnLayout: columnLayout

    contentHeight: columnLayout.implicitHeight
    interactive: contentHeight > height

    ColumnLayout {
        id: columnLayout
        width: root.width
    }
}
