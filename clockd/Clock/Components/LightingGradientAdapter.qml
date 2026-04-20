import QtQuick
import QLighting

Gradient {
    property LightingGradient lightingGradient
    stops: {
        const model = Array.from(lightingGradient.gradientStops)
        return model.map(stop => gradientStop.createObject(this, stop))
    }
}
