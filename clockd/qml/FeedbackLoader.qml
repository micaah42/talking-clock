import QtQuick 2.15
import QtQuick.Controls 2.14

Loader {
    id: loader
    asynchronous: true
    onStatusChanged: console.log('#', status)

    Popup {
        visible: loader.status === Loader.Loading
        closePolicy: Popup.NoAutoClose
        modal: true
        dim: false

        anchors.centerIn: Overlay.overlay
        height: width
        width: 128

        BusyIndicator {
            anchors.fill: parent
        }

        background: Card {
            bright: true
        }
    }
}
