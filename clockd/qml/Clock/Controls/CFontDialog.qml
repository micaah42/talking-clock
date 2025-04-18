import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import Clock 1.0
import Clock.Style

Item {
    id: ctrl

    property alias label: button.text
    property string fontFamily
    signal fontFamilyAccepted(var fontFamily)

    implicitHeight: 55

    Button {
        id: button
        anchors.fill: parent
        onClicked: popup.open()
        Material.roundedScale: Material.SmallScale
    }

    Popup {
        id: popup
        parent: window.contentItem
        height: 0.9 * window.height
        width: 0.9 * window.width
        y: (window.height - height) / 2
        x: (window.width - width) / 2

        RowLayout {
            anchors.fill: parent

            ColumnLayout {
                TextField {
                    id: search
                    placeholderText: "Search"
                    Layout.preferredWidth: 200
                    onTextChanged: FontService.families.setFilterFixedString(text)
                }

                ListView {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 200
                    clip: true

                    model: FontService.families

                    delegate: ItemDelegate {
                        width: ListView.view.width
                        highlighted: examples.family === model.display
                        onClicked: examples.family = model.display

                        CLabel {
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

                CLabel {
                    Layout.alignment: Qt.AlignHCenter
                    font.pixelSize: 32
                    font.family: examples.family
                    font.bold: true
                    color: Material.color(Material.accent)
                    text: examples.family
                }

                CLabel {
                    font.family: examples.family
                    font.pixelSize: 18
                    text: "The quick brown fox jumps over a lazy dog."
                }
                CLabel {
                    font.family: examples.family
                    font.pixelSize: 22
                    font.bold: true
                    text: "Mr. Jock, TV quiz PhD., bags few lynx."
                }
                CLabel {
                    font.family: examples.family
                    font.pixelSize: 15
                    font.italic: true
                    text: "Two driven jocks help fax my big quiz."
                }
                CLabel {
                    font.family: examples.family
                    font.pixelSize: 8
                    text: "Waltz, nymph, for quick jigs vex Bud."
                }
                CLabel {
                    font.family: examples.family
                    font.pixelSize: 14
                    text: "My girl wove six dozen plaid jackets before she quit."
                }

                Button {
                    text: "Accept"
                    Material.background: Theme.primary
                    font.family: examples.family
                    onClicked: {
                        fontFamilyAccepted(examples.family)
                        popup.close()
                    }
                }
            }
        }

        onClosed: {
            search.text = ""
            examples.family = ""
            FontService.families.setFilterFixedString("")
        }
    }
}
