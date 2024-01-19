import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

GridLayout {
    columnSpacing: 16
    rowSpacing: 16
    columns: 2
    rows: 2

    Frame {
        Layout.fillHeight: true
        Layout.fillWidth: true

        ColumnLayout {
            anchors.fill: parent
            spacing: 16

            Label {
                text: 'Talking Clock'
                font.pixelSize: 64

                Label {
                    anchors.top: parent.bottom
                    anchors.topMargin: -16

                    anchors.right: parent.right
                    anchors.rightMargin: 4

                    font.pixelSize: 18
                    text: 'by micaah42'
                }
            }

            Label {
                text: 'A fancy clock built with Qt and Yocto.'
                font.pixelSize: 18
            }

            Label {
                Layout.preferredWidth: 320
                textFormat: Text.RichText
                wrapMode: Text.WordWrap

                text: `
                We built this clock with only the finest of open source software and the most delicate of features.
                While we cannot offer you a functioning sleep button as of right now, the goal here is to design
                a fully functioning embedded-linux product and, if possible, a decent clock that can wake you up.

                This clock can also be configured using a webpage, you can reach it with http://talkingclock.home
                or http://talkingclock.lan when you are connected to the same network.
                `
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }

    Frame {
        Layout.preferredWidth: parent.width / 3
        Layout.fillHeight: true
        Label {
            text: 'b'
        }
    }

    Frame {
        Layout.preferredHeight: parent.height / 3
        Layout.fillWidth: true
        Layout.columnSpan: 2
        Label {
            text: 'c'
        }
    }
}
