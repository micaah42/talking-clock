import QtQuick 2.15
import QtQuick.Controls

Item {
    property alias sourceComponent: loader.sourceComponent
    property alias active: loader.active

    Loader {
        id: loader
        anchors.fill: parent
        asynchronous: true
        opacity: status === Loader.Ready ? 1 : 0

        Behavior on opacity {
            PropertyAnimation {}
        }
    }

    BusyIndicator {
        anchors.centerIn: parent
        opacity: loader.status === Loader.Loading ? 1 : 0

        Behavior on opacity {
            PropertyAnimation {}
        }
    }
}
