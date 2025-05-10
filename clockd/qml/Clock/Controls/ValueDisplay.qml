import QtQuick 2.14
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

ColumnLayout {
    id: root
    property alias labelText: l.text
    property alias label: l

    property alias valueText: v.text
    property alias value: v

    property int size: ValueDisplay.Large

    enum Size {
        Compact,
        Large
    }

    spacing: -2

    CLabel {
        id: l
        Layout.fillWidth: true
        size: {
            switch (root.size) {
            case ValueDisplay.Large:
                return CLabel.Medium
            case ValueDisplay.Compact:
                return CLabel.Small
            }
        }
        opacity: 0.72
    }

    CLabel {
        id: v
        Layout.fillWidth: true
        size: {
            switch (root.size) {
            case ValueDisplay.Large:
                return CLabel.Large
            case ValueDisplay.Compact:
                return CLabel.Medium
            }
        }
        bottomPadding: -4
    }
}
