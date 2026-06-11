import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Clock

import "../Style"
import "../Controls"
import "../Components"

Dialog {
    id: dialog

    anchors.centerIn: parent
    height: parent.height - 32
    width: parent.width - 32

    title: 'Weather Details'
    standardButtons: Dialog.Ok

    contentItem: ListView {
        id: list

        model: WeatherService.samples
        clip: true

        headerPositioning: ListView.OverlayHeader
        header: Item {
            id: headerItem
            height: r.implicitHeight
            width: list.width
            z: 10

            Rectangle {
                Component.onCompleted: console.log(this, width, height)
                anchors.fill: headerItem
                color: Theme.background
            }

            WeatherReportSampleItem {
                id: r
                hoverEnabled: false
                highlighted: true
                width: list.width

                time: 'Time'
                temperature: 'Temperature'
                airPressure: 'Air Pressure'
                relativeHumidity: 'Relative Humidity'
                windSpeed: 'Wind Speed'

                next1Hours.data: CLabel {
                    anchors.centerIn: parent
                    font.pixelSize: Theme.fontSizeSmall
                    text: '1 Hour'
                }
                next6Hours.data: CLabel {
                    anchors.centerIn: parent
                    font.pixelSize: Theme.fontSizeSmall
                    text: '6 Hours'
                }
                next12Hours.data: CLabel {
                    anchors.centerIn: parent
                    font.pixelSize: Theme.fontSizeSmall
                    text: '12 Hours'
                }
            }
        }

        component WeatherReportNextHoursItem: RowLayout {
            id: d1
            property WeatherReportNextHours nextHours
            visible: nextHours
            spacing: 16

            Icon {
                text: d1.nextHours && d1.nextHours.symbolCode ? Icons[d1.nextHours.symbolCode] : ''
                font.pixelSize: Theme.fontSizeSmall
            }
            CLabel {
                text: d1.nextHours ? d1.nextHours.precipitationAmount : ''
                font.pixelSize: Theme.fontSizeSmall
            }
        }

        delegate: WeatherReportSampleItem {
            id: d
            width: list.width
            property WeatherReportSample sample: modelData

            time: sample.time.toLocaleString(Qt.locale(), Locale.NarrowFormat)
            temperature: sample.airTemperature
            airPressure: sample.airPressureAtSeaLevel
            relativeHumidity: sample.relativeHumidity
            windSpeed: sample.windSpeed

            next1Hours.data: WeatherReportNextHoursItem {
                anchors.centerIn: parent
                nextHours: d.sample.next1Hours
            }
            next6Hours.data: WeatherReportNextHoursItem {
                anchors.centerIn: parent
                nextHours: d.sample.next6Hours
            }
            next12Hours.data: WeatherReportNextHoursItem {
                anchors.centerIn: parent
                nextHours: d.sample.next12Hours
            }
        }
    }
}
