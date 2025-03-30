import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock 1.0
import Clock.Controls 1.0

RowLayout {
    anchors.fill: parent
    spacing: 8

    Frame {
        Layout.preferredWidth: 330
        Layout.fillHeight: true

        ColumnLayout {
            anchors.fill: parent
            spacing: 16

            RowLayout {
                Layout.fillWidth: true

                CLabel {
                    text: 'Talking Clock'
                    font.pixelSize: 32

                    CLabel {
                        anchors.top: parent.bottom
                        anchors.topMargin: -8

                        font.pixelSize: 18
                        text: 'by micaah42'
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                Image {
                    Layout.preferredHeight: parent.height
                    Layout.preferredWidth: height
                    fillMode: Image.PreserveAspectFit
                    source: 'qrc:/icon-latest.png'

                    MouseArea {
                        anchors.fill: parent
                        onClicked: rotate.start()
                    }

                    RotationAnimator on rotation {
                        id: rotate
                        running: false
                        duration: 2500
                        easing.type: Easing.InOutElastic
                        from: 0
                        to: 360
                    }
                }
            }

            CLabel {
                text: `A fancy clock built with Qt and Yocto.\nVerison ${"0.1.0"}`
                font.pixelSize: 18
            }

            Flickable {
                contentHeight: textColumn.implicitHeight + 16
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true

                ColumnLayout {
                    id: textColumn
                    width: parent.width

                    CLabel {
                        Layout.fillWidth: true
                        textFormat: Text.RichText
                        wrapMode: Text.WordWrap

                        text: `
                        <p>
                        We built this clock with only the finest of open source software and the most delicate of features.
                        While we cannot offer you a functioning sleep button as of right now, the goal here is to design
                        a fully functioning embedded-linux product and, if possible, a decent clock that can wake you up.
                        </p>
                        `
                    }

                    CLabel {
                        Layout.fillWidth: true
                        textFormat: Text.RichText
                        wrapMode: Text.WordWrap
                        font.family: 'Mono'
                        font.pixelSize: 9

                        text: `
                        <p>
                        Copyright (C) 2024 Michael Wipplinger
                        </p>

                        <p>
                        This program is free software: you can redistribute it and/or modify
                        it under the terms of the GNU General Public License as published by
                        the Free Software Foundation, either version 3 of the License, or
                        (at your option) any later version.
                        </p>

                        <p>
                        This program is distributed in the hope that it will be useful,
                        but WITHOUT ANY WARRANTY; without even the implied warranty of
                        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
                        GNU General Public License for more details.
                        </p>

                        <p>
                        You should have received a copy of the GNU General Public License
                        along with this program.  If not, see https://www.gnu.org/licenses/.
                        </p>
                        `
                    }
                }
            }
        }
    }

    Frame {
        Layout.fillHeight: true
        Layout.fillWidth: true

        ColumnLayout {
            anchors.fill: parent
            spacing: 8

            CLabel {
                text: 'Open Source Licenses'
                font.pixelSize: 32
            }

            CTextField {
                Component.onDestruction: About.packages.setFilterWildcard('')
                onTextEdited: About.packages.setFilterWildcard(text)
                placeholderText: 'Search packages'
                Layout.fillWidth: true
                Layout.topMargin: 2
            }

            ListView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true

                Popup {
                    id: popup
                    parent: window.contentItem
                    scale: window.s === 0 ? 1 : 1 / window.s
                    y: (window.height - height) / 2
                    x: (window.width - width) / 2
                    height: 0.8 * window.height
                    width: 0.8 * window.width
                    dim: false

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16

                        RowLayout {
                            Layout.fillWidth: true
                            CLabel {
                                text: list.packageName
                                font.pixelSize: 24
                                font.bold: true
                            }
                            Item {
                                Layout.fillWidth: true
                            }

                            ToolButton {
                                text: 'x'
                                onClicked: popup.close()
                            }
                        }

                        ListView {
                            id: list
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            clip: true

                            ScrollBar.vertical: ScrollBar {}

                            property string packageName

                            delegate: CLabel {
                                font.family: 'Mono'
                                width: list.width
                                text: modelData
                                clip: true
                            }
                        }
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    opacity: 0.1
                    radius: 3
                }

                model: About.packages
                delegate: ItemDelegate {
                    text: model.display
                    width: ListView.view.width
                    onClicked: {
                        list.model = About.licenseText(model.display)
                        list.packageName = model.display
                        popup.open()
                    }
                }
            }
        }
    }
}
