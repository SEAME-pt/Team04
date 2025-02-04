import QtQuick
import QtQuick.Controls
import Constants

Item {
    id: root

    property double rpm
    property double temperature: 0

    implicitHeight: 250
    implicitWidth: 250

    CircularSlider {
        id: speedSlider
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        endAngle: 320
        height: parent.height
        interactive: false
        maxValue: Constants.tachometerMaxValue
        progressColor: "#a7b4fe"
        progressWidth: 16
        rotation: 180
        snap: false
        startAngle: 40
        trackColor: "#5d5d5d"
        trackWidth: 16
        value: rpm
        width: parent.width
    }
    ClusterWidget {
        id: rpmWidget
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        backgroundColor: "transparent"
        description: qsTr("x100 rpm")
        fontColor: "#ffffff"
        fontSize: 112
        size: parent.height - 32
        value: (rpm / 100).toFixed(0)
    }
    CircularSlider {
        id: temperatureSlider
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        endAngle: 148
        height: parent.height
        interactive: false
        maxValue: Constants.temperatureMaxLevel
        progressColor: temperature > Constants.temperatureWarningLevel ? "#ff0000" : "#a7b4fe"
        progressWidth: 8
        snap: false
        startAngle: 212
        trackColor: "#5d5d5d"
        trackWidth: 8
        value: temperature
        width: parent.width
    }
    Text {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#ffffff"
        font.pixelSize: 16
        text: temperature.toFixed(0) + qsTr("ºC")
    }
}
