import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls

import Clock

import "../Style"
import "../Controls"
import "../Components"

Item {
    property real contentHeight: column.implicitHeight

    Flickable {
        anchors.fill: parent
        contentHeight: column.implicitHeight

        ColumnLayout {
            id: column
            width: parent.width
            spacing: 16

            CLabel {
                Layout.bottomMargin: 4
                size: CLabel.XLarge
                text: `Today is...`
            }

            Repeater {
                model: ActionDayService.days

                delegate: ItemDelegate {
                    Layout.fillWidth: true
                    Layout.rightMargin: -16
                    Layout.leftMargin: -16

                    contentItem: SimpleListItem {
                        id: delegate
                        property ActionDay actionDay: modelData
                        icon: Icons[actionDay.icon] || Icons.calendar_month
                        title: actionDay.name
                        description: actionDay.desc
                        width: parent.width
                    }

                    onClicked: {
                        dialog.actionDay = delegate.actionDay
                        dialog.open()
                    }
                }
            }
        }
    }

    Dialog {
        id: dialog

        property LinkPreview linkPreview
        property ActionDay actionDay

        title: actionDay?.name || ''
        onClosed: actionDay = null

        parent: Overlay.overlay
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) / 2)
        modal: true
        z: 1

        onAboutToShow: {
            dialog.linkPreview = LinkPreviewService.getLinkPreview(dialog.actionDay.link)
        }

        contentItem: FeedbackLoader {
            active: dialog.linkPreview && dialog.linkPreview.status === LinkPreview.Success
            implicitHeight: item?.implicitHeight || 0
            width: parent.width

            sourceComponent: ColumnLayout {
                id: content

                Image {
                    Layout.preferredWidth: content.width
                    Layout.preferredHeight: (sourceSize.height * width) / sourceSize.width
                    source: dialog.linkPreview.ogImage
                }

                CLabel {
                    Layout.fillWidth: true
                    text: dialog.linkPreview.ogDescription
                    wrapMode: Text.Wrap
                }
            }
        }
    }
}
