import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

import Clock
import QLighting

import "../Style"
import "../Controls"

ColumnLayout {
    id: root

    property var lightMode

    spacing: 16

    Loader {
        Layout.fillWidth: true
        Layout.fillHeight: true

        sourceComponent: {
            if (!lightMode) {
                return null
            }

            switch (lightMode.type) {
            case LightMode.TypeStatic:
                return staticLightComponent
            case LightMode.TypeWaving:
                return wavingLightComponent
            case LightMode.TypeMonoRotation:
                return monoRotationLightComponent
            case LightMode.TypePerlin:
                return perlinLightComponent
            case LightMode.TypePerlin2D:
                return perlinLight2DComponent
            default:
                console.error('unsupported type!')
                return null
            }
        }

        Component {
            id: staticLightComponent
            StaticLightEdit {
                lightMode: root.lightMode
            }
        }

        Component {
            id: wavingLightComponent
            WavingLightEdit {
                lightMode: root.lightMode
            }
        }

        Component {
            id: monoRotationLightComponent
            MonoRotationLightEdit {
                lightMode: root.lightMode
            }
        }

        Component {
            id: perlinLightComponent
            PerlinLightEdit {
                lightMode: root.lightMode
            }
        }

        Component {
            id: perlinLight2DComponent
            PerlinLight2DEdit {
                lightMode: root.lightMode
            }
        }
    }
}
