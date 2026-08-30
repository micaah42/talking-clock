import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

import Clock

import "../Style"
import "../Controls"
import "../Components"
import "../Weather"

ColumnLayout {
    id: weatherDisplay

    readonly property Item dialogParent: Window.contentItem
    readonly property WeatherService weatherService: WeatherService
    readonly property string currentSymbol: weatherService.currentSymbol

    width: parent.width
    spacing: 8

    RowLayout {
        Layout.fillWidth: true
        spacing: 16

        CLabel {
            Layout.fillWidth: true
            text: `Weather in ${weatherService.cityName}...`
            size: CLabel.XLarge
            Layout.bottomMargin: 4
        }

        CToolButton {
            Layout.margins: -16
            onClicked: {
                if (weatherService.status === WeatherService.LocationError)
                    weatherService.fetchIPLocation()
                else
                    weatherService.fetchWeatherData()
            }
            text: Icons.refresh
        }
        CToolButton {
            Layout.margins: -16
            onClicked: dialog.open()
            text: Icons.open_in_new
        }
    }

    WeatherDetailDialog {
        id: dialog
        parent: weatherDisplay.dialogParent
    }

    component WeatherLine: ColumnLayout {
        id: weatherLine
        property string title
        property string icon
        property string precipitationAmount
        property string precipitationProbability
        property string temperature
        property string subTemperature
        property string windspeed
        property string subWindspeed

        spacing: 0

        RowLayout {
            CLabel {
                Layout.preferredWidth: weatherLine.width / 4
                text: weatherLine.title
            }
            Rectangle {
                // Layout.alignment: Qt.AlignBottom
                Layout.preferredHeight: 1.5
                Layout.fillWidth: true
                radius: height / 2

                color: Theme.foreground
                opacity: Theme.o24
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            Icon {
                Layout.rightMargin: 8
                font.pixelSize: Theme.fontSizeXXXLarge
                opacity: Theme.o84
                text: weatherLine.icon
            }
            WeatherValueDisplay {
                icon: Icons.device_thermostat
                value: weatherLine.temperature
                subValue: weatherLine.subTemperature
                unit: '°C'
            }
            WeatherValueDisplay {
                value: weatherLine.windspeed
                subValue: weatherLine.subWindspeed
                unit: 'm/s'
                icon: Icons.air
            }
            WeatherValueDisplay {
                value: Theme.roundWithPrecision(weatherLine.precipitationAmount)
                subValue: {
                    if (weatherLine.precipitationProbability > 0)
                        return `${Theme.roundWithPrecision(weatherLine.precipitationProbability)}%`
                    else
                        return ''
                }
                icon: Icons.humidity_high
                unit: 'mm'
            }
        }
    }

    component SummaryWeatherLine: WeatherLine {
        property var summary

        icon: Icons[summary["commonSymbol"]]
        temperature: summary["maxTemperature"]
        subTemperature: summary["minTemperature"]
        windspeed: summary["maxWindSpeed"]
        subWindspeed: summary["minWindSpeed"]
        precipitationAmount: summary["totalFuturePrecipitationAmount"]
        precipitationProbability: summary["maxProbabilityOfPrecipitation"]
    }

    WeatherLine {
        title: 'Now'
        icon: weatherService.current.next1Hours.symbolCode
        temperature: weatherService.current.airTemperature
        precipitationAmount: weatherService.current.next1Hours.precipitationAmount
        precipitationProbability: weatherService.current.next1Hours.probabilityOfPrecipitation
        windspeed: weatherService.current.windSpeed
    }
    SummaryWeatherLine {
        title: 'Next 12h'
        summary: WeatherService.summarizeWeatherSamples(weatherService.next12HoursSamples)
    }
    SummaryWeatherLine {
        title: 'Tomorrow'
        summary: WeatherService.summarizeWeatherSamples(weatherService.tomorrowSamples)
    }

    component WeatherValueDisplay: Item {
        id: weatherValueDisplay

        property string value
        property string subValue
        property string icon
        property string unit
        property real margins: 8

        implicitHeight: label.implicitHeight
        Layout.fillWidth: true

        Icon {
            anchors.margins: weatherValueDisplay.margins
            anchors.verticalCenterOffset: 2
            anchors.verticalCenter: label.verticalCenter
            anchors.right: label.left

            text: weatherValueDisplay.icon
            font.pixelSize: Theme.fontSizeSmall
            opacity: Theme.o72
        }

        ColumnLayout {
            id: label
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: -6

            CLabel {
                Layout.alignment: Qt.AlignHCenter
                text: weatherValueDisplay.value
                opacity: Theme.o84
                size: CLabel.XLarge
            }
            CLabel {
                Layout.alignment: Qt.AlignHCenter
                text: weatherValueDisplay.subValue
                size: CLabel.Small
                opacity: Theme.o72
                visible: text
            }
        }

        CLabel {
            anchors.margins: weatherValueDisplay.margins
            anchors.verticalCenter: label.verticalCenter
            anchors.left: label.right
            text: weatherValueDisplay.unit
            opacity: Theme.o72
            size: CLabel.Small
        }
    }

    Item {
        Layout.fillHeight: true
        Layout.fillWidth: true

        RowLayout {
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            opacity: Theme.o56
            visible: false
            spacing: 8

            CLabel {
                id: hint
                text: `updated: ${Theme.timeString(weatherService.timestamp)}`
                size: CLabel.Small
            }

            StackLayout {
                id: statusLayout

                Icon {
                    id: icon
                    text: Icons.error
                }

                BusyIndicator {
                    implicitHeight: icon.height
                    implicitWidth: icon.width
                }

                Icon {
                    text: Icons.done
                }

                ToolTip.text: {
                    switch (weatherService.status) {
                    case WeatherService.LocationError:
                        return 'Location Error'
                    case WeatherService.WeatherError:
                        return 'Weather Error'
                    case WeatherService.Fetching:
                        return 'Fetching...'
                    case WeatherService.Valid:
                        return 'Valid'
                    }
                }
            }
            Timer {
                property int currentIndex: {
                    switch (weatherService.status) {
                    case WeatherService.LocationError:
                    case WeatherService.WeatherError:
                        return 0
                    case WeatherService.Fetching:
                        return 1
                    case WeatherService.Valid:
                        return 2
                    }
                }

                onCurrentIndexChanged: {
                    if (running)
                        return
                    statusLayout.currentIndex = currentIndex
                    start()
                }

                onTriggered: statusLayout.currentIndex = currentIndex
                interval: 420
            }
        }
    }
}
