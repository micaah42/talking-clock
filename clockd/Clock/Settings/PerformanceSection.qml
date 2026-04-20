import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Clock
import QPerformanceQml

import "../Style"
import "../Controls"

Section {
    id: performanceSection
    title: 'Performance'
    icon: Icons.monitoring
    property bool isCurrentItem: SwipeView.isCurrentItem

    Flickable {
        id: flickable

        contentHeight: content.implicitHeight
        Layout.fillHeight: true
        Layout.fillWidth: true
        clip: true

        PerformanceDisplay {
            id: content
            width: flickable.width
        }
    }
}
