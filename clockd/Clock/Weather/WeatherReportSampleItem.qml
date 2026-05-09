import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

import Clock

import "../Style"
import "../Controls"
import "../Components"

ItemDelegate {
    id: root
    rightPadding: 4
    leftPadding: 8
    padding: 0

    property alias time: _time.text
    property alias temperature: _temperature.text
    property alias airPressure: _airPressure.text
    property alias relativeHumidity: _relativeHumidity.text
    property alias windSpeed: _windSpeed.text

    property alias next1Hours: _next1Hours
    property alias next6Hours: _next6Hours
    property alias next12Hours: _next12Hours

    component MiniLabel: CLabel {
        elide: Text.ElideMiddle
        size: CLabel.Small
    }

    contentItem: RowLayout {
        width: parent.width

        spacing: 8

        MiniLabel {
            id: _time
            Layout.preferredWidth: root.width / 6
        }
        MiniLabel {
            id: _temperature
            Layout.preferredWidth: root.width / 12
        }
        MiniLabel {
            id: _airPressure
            Layout.preferredWidth: root.width / 12
        }
        MiniLabel {
            id: _relativeHumidity
            Layout.preferredWidth: root.width / 8
        }
        MiniLabel {
            id: _windSpeed
            Layout.preferredWidth: root.width / 12
        }

        Item {
            id: _next1Hours
            Layout.fillWidth: true
        }
        Item {
            id: _next6Hours
            Layout.fillWidth: true
        }
        Item {
            id: _next12Hours
            Layout.fillWidth: true
        }
    }
}
