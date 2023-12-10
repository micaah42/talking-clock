import QtQuick 2.15
import QtQuick.Layouts 1.15

import ".."
import Clock 1.0
import Controls 1.0

Item {
    ListView {
        anchors.fill: parent
        model: DataService.datasets
        delegate: Card {
            bright: true
            GridLayout {
                CLabel {
                    text: model.name
                }
                CLabel {
                    text: model.type
                }
                CLabel {
                    text: model.fetches
                }
                CLabel {
                    text: model.bytes
                }
            }
        }
    }
}
