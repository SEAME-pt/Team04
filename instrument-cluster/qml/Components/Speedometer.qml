import QtQuick
import QtQuick.Controls
import Constants

Item {
    id: root

    property double battery: 0
    property double speed

    implicitHeight: 250
    implicitWidth: 250

    CircularSlider {
        id: speedSlider
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        endAngle: 320
        height: parent.height
        interactive: false
        maxValue: Constants.speedometerMaxValue
        progressColor: "#a7b4fe"
        progressWidth: 16
        rotation: 180
        snap: false
        startAngle: 40
        trackColor: "#5d5d5d"
        trackWidth: 16
        value: speed
        width: parent.width
    }
    ClusterWidget {
        id: speedWidget
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        backgroundColor: "transparent"
        description: qsTr("km/h")
        fontColor: "#ffffff"
        fontSize: 112
        size: parent.height - 32
        value: speed
    }
    CircularSlider {
        id: batterySlider
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        endAngle: 148
        height: parent.height
        interactive: false
        maxValue: 100
        progressColor: battery < Constants.batteryWarningLevel ? "#ff0000" : "#a7b4fe"
        progressWidth: 8
        snap: false
        startAngle: 212
        trackColor: "#5d5d5d"
        trackWidth: 8
        value: battery
        width: parent.width
    }
    Text {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#ffffff"
        font.pixelSize: 16
        text: battery.toFixed(0) + qsTr(" %")
    }
}
