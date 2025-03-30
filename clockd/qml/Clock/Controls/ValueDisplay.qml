import QtQuick 2.14
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

ColumnLayout {
    property alias labelText: l.text
    property alias label: l

    property alias valueText: v.text
    property alias value: v

    spacing: -2

    CLabel {
        id: l
        Layout.fillWidth: true
        size: CLabel.Medium
        opacity: 0.72
    }

    CLabel {
        id: v
        Layout.fillWidth: true
        size: CLabel.Large
        bottomPadding: -4
    }
}
