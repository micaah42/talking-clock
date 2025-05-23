import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import Clock
import Clock.Style
import Clock.Controls

Popup {
    id: popup

    property string fontFamily: ''
    signal fontFamilyAccepted(var string)

    parent: window.contentItem
    anchors.centerIn: parent
    height: 0.9 * window.height
    width: 0.9 * window.width

    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            Layout.minimumWidth: Theme.controlWidth
            Layout.maximumWidth: Theme.controlWidth

            TextField {
                id: search
                Layout.fillWidth: true
                onTextChanged: FontService.families.setFilterFixedString(text)
                placeholderText: "Search"
            }

            ListView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true

                model: FontService.families

                delegate: ItemDelegate {
                    highlighted: fontFamily === model.display
                    onClicked: fontFamily = model.display
                    width: ListView.view.width
                    text: model.display
                }
            }

            Button {
                Layout.fillWidth: true
                Material.background: Theme.primary
                text: "Accept"

                onClicked: {
                    fontFamilyAccepted(fontFamily)
                    popup.close()
                }
            }
        }

        Component {
            id: examplesComponent
            ColumnLayout {
                id: examples
                property string family
                visible: family !== ""
                spacing: 20

                CLabel {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter
                    font.pixelSize: 32
                    font.family: examples.family
                    font.bold: true
                    color: Material.color(Material.accent)
                    text: examples.family
                }

                CLabel {
                    Layout.fillWidth: true
                    text: "The quick brown fox jumps over a lazy dog."
                    font.family: examples.family
                    font.pixelSize: 18
                    wrapMode: Text.Wrap
                }
                CLabel {
                    Layout.fillWidth: true
                    text: "Mr. Jock, TV quiz PhD., bags few lynx."
                    font.family: examples.family
                    font.pixelSize: 22
                    font.bold: true
                    wrapMode: Text.Wrap
                }
                CLabel {
                    Layout.fillWidth: true
                    text: "Two driven jocks help fax my big quiz."
                    font.family: examples.family
                    font.pixelSize: 15
                    font.italic: true
                    wrapMode: Text.Wrap
                }
                CLabel {
                    Layout.fillWidth: true
                    text: "Waltz, nymph, for quick jigs vex Bud."
                    font.family: examples.family
                    font.pixelSize: 8
                    wrapMode: Text.Wrap
                }
                CLabel {
                    Layout.fillWidth: true
                    text: "My girl wove six dozen plaid jackets before she quit."
                    font.family: examples.family
                    font.pixelSize: 14
                    wrapMode: Text.Wrap
                }
            }
        }

        Loader {
            Layout.fillWidth: true
            sourceComponent: examplesComponent
            onLoaded: item.family = Qt.binding(() => fontFamily)
            active: fontFamily.length
        }
        Loader {
            Layout.fillWidth: true
            sourceComponent: examplesComponent
            onLoaded: item.family = Qt.binding(() => FontService.family)
        }
    }

    onClosed: {
        FontService.families.setFilterFixedString("")
        search.text = ""
        fontFamily = ""
    }
}
