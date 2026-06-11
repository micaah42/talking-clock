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
            text: Icons.more_vert
        }
    }

    Dialog {
        id: dialog
        anchors.centerIn: weatherDisplay.dialogParent
        parent: weatherDisplay.dialogParent
        height: weatherDisplay.dialogParent.height - 32
        width: weatherDisplay.dialogParent.width - 32

        title: 'Weather Details'
        standardButtons: Dialog.Ok

        contentItem: ListView {
            id: list

            model: weatherService.samples
            clip: true

            headerPositioning: ListView.OverlayHeader
            header: Item {
                height: r.implicitHeight
                width: list.width
                z: 1

                Rectangle {
                    id: rec
                    Component.onCompleted: console.log(rec, rec.width, rec.height)
                    anchors.fill: parent
                    color: 'black'
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

    component WeatherLine: ColumnLayout {
        id: weatherLine
        property string title
        property string icon
        property string precipitationAmount
        property string temperature
        property string subTemperature
        property string windspeed
        property string subWindspeed

        spacing: -4

        CLabel {
            text: weatherLine.title
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 8
            Icon {
                font.pixelSize: Theme.fontSizeXXXLarge
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
                value: weatherLine.precipitationAmount
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
    }

    WeatherLine {
        title: 'Now'
        icon: weatherService.current.next1Hours.symbolCode
        temperature: weatherService.current.airTemperature
        precipitationAmount: weatherService.current.next1Hours.precipitationAmount
        windspeed: weatherService.current.windSpeed
    }
    SummaryWeatherLine {
        title: 'Today'
        summary: WeatherService.summarizeWeatherSamples(weatherService.todaySamples)
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
        }

        ColumnLayout {
            id: label
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: -4

            CLabel {
                Layout.alignment: Qt.AlignHCenter
                text: weatherValueDisplay.value
                size: CLabel.XLarge
            }
            CLabel {
                Layout.alignment: Qt.AlignHCenter
                text: weatherValueDisplay.subValue
                size: CLabel.Small
                visible: text
            }
        }

        CLabel {
            anchors.margins: weatherValueDisplay.margins
            anchors.verticalCenter: label.verticalCenter
            anchors.left: label.right

            text: weatherValueDisplay.unit
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
