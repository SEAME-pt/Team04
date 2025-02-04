/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls
import Constants
import Components
import ClusterData

Rectangle {
    id: rectangle
    color: Constants.backgroundColor
    height: Constants.height
    states: []
    width: Constants.width

    Speedometer {
        id: speedometer
        anchors.left: parent.left
        anchors.leftMargin: 160
        anchors.verticalCenter: parent.verticalCenter
        battery: Backend.battery
        height: 300
        speed: Backend.speedComputed
        width: 300
    }
    Tachometer {
        id: tachometer
        anchors.right: parent.right
        anchors.rightMargin: 160
        anchors.verticalCenter: parent.verticalCenter
        height: 300
        rpm: Backend.rpm
        temperature: Backend.temperature
        width: 300
    }
    Text {
        id: currentTime
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 16
        color: "#ffffff"
        font.pixelSize: 14
        text: Backend.hours + qsTr(":") + Backend.minutes
    }
}
