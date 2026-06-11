import QtQuick
import QtQuick.Layouts

import Clock
import "../Style"
import "../Controls"

GridLayout {
    default property list<KeyValueObject> keyValueObjects
    columns: 2

    Repeater {
        model: 2 * keyValueObjects.length
        delegate: CLabel {
            property KeyValueObject keyValueObject: keyValueObject[Math.floor(index / 2)]
            readonly property bool isValue: index % 2

            Layout.fillWidth: true
            text: isValue ? keyValueObject.value : keyValueObject.key
            opacity: isValue ? 1 : Theme.o72
        }
    }
}
