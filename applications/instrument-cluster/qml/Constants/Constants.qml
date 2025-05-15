pragma Singleton
import QtQuick

QtObject {
    readonly property color backgroundColor: "#272727"
    readonly property int batteryWarningLevel: 20
    readonly property int height: 400
    readonly property int speedometerMaxValue: 16
    readonly property int tachometerMaxValue: 3000
    readonly property int temperatureMaxLevel: 110
    readonly property int temperatureMinLevel: 0
    readonly property int temperatureWarningLevel: 90
    readonly property int width: 1280
}
