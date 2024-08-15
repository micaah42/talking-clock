import QtQuick 2.14
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

ColumnLayout {
    property alias labelText: l.text
    property alias label: l

    property alias valueText: v.text
    property alias value: v

    spacing: 0

    Label {
        id: l
        font.pixelSize: 18
        opacity: 0.72
    }

    Label {
        id: v
        font.pixelSize: 32
        bottomPadding: -4
    }
}
