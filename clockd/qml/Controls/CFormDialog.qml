import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import Clock 1.0

Item {
    id: ctrl

    property string value
    property alias label: button.text

    implicitHeight: 55

    Button {
        id: button
        anchors.fill: parent
        onClicked: popup.open()
    }

    Popup {
        id: popup
        parent: Overlay.overlay
        anchors.centerIn: Overlay.overlay
        width: 0.9 * window.width
        height: 0.9 * window.height

        RowLayout {
            anchors.fill: parent

            ColumnLayout {
                TextField {
                    id: search
                    placeholderText: "Search"
                    Layout.preferredWidth: 200
                    onTextChanged: fontService.families.setFilterFixedString(text)
                }

                ListView {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 200
                    clip: true

                    model: fontService.families

                    delegate: ItemDelegate {
                        width: ListView.view.width
                        highlighted: examples.family === model.display
                        onClicked: examples.family = model.display

                        Label {
                            anchors.centerIn: parent
                            text: model.display
                        }
                    }
                }
            }

            ColumnLayout {
                id: examples
                property string family
                visible: family !== ""
                spacing: 20

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    font.pixelSize: 32
                    font.family: examples.family
                    font.bold: true
                    color: Material.color(Material.accent)
                    text: examples.family
                }

                Label {
                    font.family: examples.family
                    font.pixelSize: 18
                    text: "The quick brown fox jumps over a lazy dog."
                }
                Label {
                    font.family: examples.family
                    font.pixelSize: 22
                    font.bold: true
                    text: "Mr. Jock, TV quiz PhD., bags few lynx."
                }
                Label {
                    font.family: examples.family
                    font.pixelSize: 15
                    font.italic: true
                    text: "Two driven jocks help fax my big quiz."
                }
                Label {
                    font.family: examples.family
                    font.pixelSize: 8
                    text: "Waltz, nymph, for quick jigs vex Bud."
                }
                Label {
                    font.family: examples.family
                    font.pixelSize: 14
                    text: "My girl wove six dozen plaid jackets before she quit."
                }

                Button {
                    text: "Accept"
                    Material.background: ColorService.primary
                    font.family: examples.family
                    onClicked: {
                        ctrl.value = examples.family
                        popup.close()
                    }
                }
            }
        }

        onClosed: {
            search.text = ""
            examples.family = ""
            fontService.families.setFilterFixedString("")
        }
    }
}
