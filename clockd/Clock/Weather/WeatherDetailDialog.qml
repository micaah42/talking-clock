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

    readonly property WeatherService weatherService: WeatherService
    readonly property var next7Hours: weatherService.samples.slice(0, 7)

    anchors.centerIn: parent
    height: parent ? parent.height - 32 : 0
    width: parent ? parent.width - 32 : 0

    header: RowLayout {
        height: implicitHeight - 24
        spacing: 0

        Icon {
            Layout.leftMargin: dialog.leftPadding || dialog.padding
            Layout.rightMargin: 16
            font.pixelSize: Theme.fontSizeXLarge
            text: Icons.location_on
        }
        CLabel {
            Layout.fillWidth: true
            text: `Weather in ${weatherService.cityName}...`
            size: CLabel.XLarge
        }
        CToolButton {
            Layout.rightMargin: -16
            Layout.margins: 16
            text: Icons.settings
        }
        CToolButton {
            Layout.rightMargin: (dialog.rightPadding || dialog.padding) - 16
            Layout.margins: 16
            onClicked: dialog.close()
            text: Icons.close
        }
    }

    standardButtons: Dialog.NoButton
    closePolicy: Popup.NoAutoClose
    modal: true

    contentItem: ColumnLayout {
        spacing: 8

        CLabel {
            text: 'Hourly Preview'
        }
        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            Repeater {
                model: dialog.next7Hours
                delegate: Frame {
                    Layout.fillWidth: true
                    implicitWidth: 0

                    contentItem: ForecastTile {
                        time: modelData.time.toLocaleTimeString(Qt.locale(), Locale.ShortFormat)
                        icon: modelData.next1Hours ? Icons[modelData.next1Hours.symbolCode] : ''
                        temperature: `${Theme.roundWithPrecision(modelData.airTemperature)}°`
                        precipitation: `${Theme.roundWithPrecision(modelData.next1Hours.precipitationAmount)} mm`
                        // windspeed: `${Theme.roundWithPrecision(modelData.windSpeed)} m/s`
                    }
                }
            }
        }

        CLabel {
            text: '7-Day Forecast'
        }
        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            Repeater {
                model: 7
                delegate: Frame {
                    Layout.fillWidth: true
                    implicitWidth: 0

                    contentItem: ForecastTile {
                        property date day: new Date(new Date().getTime() + 24 * 60 * 60 * 1000 * index)
                        property var summary: {
                            const samples = weatherService.collectWeatherSamples(weatherService.samples, day)
                            return weatherService.summarizeWeatherSamples(samples)
                        }

                        time: {
                            const weekday = day.toLocaleDateString(Qt.locale(), 'ddd')
                            const shortDate = day.toLocaleDateString(Qt.locale(), Locale.NarrowFormat)
                            return `${weekday}, ${shortDate}`
                        }
                        icon: Icons[summary.commonSymbol]

                        temperature: {
                            const roundedMin = Theme.roundWithPrecision(summary.minTemperature)
                            const roundedMax = Theme.roundWithPrecision(summary.maxTemperature)
                            return `${roundedMin}°-${roundedMax}°`
                        }

                        precipitation: `${Theme.roundWithPrecision(summary.totalFuturePrecipitationAmount)} mm`
                        // windspeed: `${Theme.roundWithPrecision(summary.maxWindSpeed)} m/s`
                    }
                }
            }
        }
    }

    component ForecastTile: ColumnLayout {
        id: tile
        property string time
        property string icon
        property string temperature
        property string windspeed
        property string precipitation
        property string details
        spacing: 2

        CLabel {
            Layout.alignment: Qt.AlignHCenter
            text: tile.time
            size: CLabel.Small
            opacity: Theme.o72
        }
        Icon {
            Layout.alignment: Qt.AlignHCenter
            font.pixelSize: Theme.fontSizeXLarge
            text: tile.icon
        }
        CLabel {
            Layout.alignment: Qt.AlignHCenter
            text: tile.temperature
        }
        // CLabel {
        //     Layout.alignment: Qt.AlignHCenter
        //     text: tile.windspeed
        //     size: CLabel.Small
        //     opacity: Theme.o72
        //     visible: text
        // }
        CLabel {
            Layout.alignment: Qt.AlignHCenter
            text: tile.precipitation
            size: CLabel.Small
            color: Theme.primary
            opacity: Theme.o72
            visible: text
        }
        // CLabel {
        //     Layout.alignment: Qt.AlignHCenter
        //     text: tile.details
        //     size: CLabel.Small
        //     opacity: Theme.o56
        //     visible: text
        // }
    }
}
