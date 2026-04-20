import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQml.Models

import Clock

import "../Style"
import "../Main"
import "../Controls"

Section {
    id: root

    title: 'Side Bar'
    icon: Icons.dock_to_right

    RowLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true

        DelegateModel {
            id: visualModel
            model: QObjectListModel {
                asList: SideBarConfiguration.availableSideBarSections
            }

            delegate: Item {
                id: delegateRoot

                width: list.width
                height: 42

                // This allows the item to "pop out" of the list layout when dragged
                readonly property bool isHeld: mouseArea.drag.active

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    drag.target: contentItem
                    drag.axis: Drag.YAxis // Restrict to vertical movement

                    onReleased: contentItem.Drag.drop()
                }

                Rectangle {
                    id: contentItem
                    width: delegateRoot.width
                    height: delegateRoot.height - 4
                    color: colorCode
                    radius: 5

                    // Visual feedback when dragging
                    opacity: delegateRoot.isHeld ? 0.8 : 1.0
                    scale: delegateRoot.isHeld ? 1.05 : 1.0
                    z: delegateRoot.isHeld ? 100 : 1

                    Drag.active: mouseArea.drag.active
                    Drag.source: mouseArea
                    Drag.hotSpot.x: width / 2
                    Drag.hotSpot.y: height / 2

                    Text {
                        anchors.centerIn: parent
                        text: name
                        color: "white"
                        font.bold: true
                    }

                    // Smoothly slide items into their new positions
                    Behavior on y {
                        enabled: !delegateRoot.isHeld
                        NumberAnimation {
                            duration: 200
                            easing.type: Easing.OutCubic
                        }
                    }
                }

                DropArea {
                    anchors.fill: parent
                    onEntered: drag => {
                                   // Logic: Swap the dragged item with the one we just entered
                                   visualModel.items.move(drag.source.DelegateModel.itemsIndex,
                                                          delegateRoot.DelegateModel.itemsIndex)
                               }
                }
            }
        }

        ListView {
            id: list
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: visualModel
            spacing: 4
        }
    }
}
