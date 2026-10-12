import QtQuick

import Clock

Rectangle {
    property bool horizontal: true
    property real thickness: 1.5

    implicitHeight: thickness
    implicitWidth: thickness
    radius: thickness / 2

    color: Theme.foreground
    opacity: Theme.o24
}
