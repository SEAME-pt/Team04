/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls

Rectangle {
    id: rectangle
    color: "white"
    height: 400
    width: 1280

    states: [
        State {
            name: "clicked"
            when: button.checked

            PropertyChanges {
                target: label
                text: qsTr("Button Checked")
            }
        }
    ]

    Button {
        id: button
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        checkable: true
        text: qsTr("Press me")

        Connections {
            target: button

            onClicked: animation.start()
        }
    }
    Text {
        id: label
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: button.bottom
        anchors.topMargin: 45
        text: qsTr("Hello Instrument Cluster")

        SequentialAnimation {
            id: animation
            ColorAnimation {
                id: colorAnimation1
                from: parent.backgroundColor
                property: "color"
                target: rectangle
                to: "#2294c6"
            }
            ColorAnimation {
                id: colorAnimation2
                from: "#2294c6"
                property: "color"
                target: rectangle
                to: parent.backgroundColor
            }
        }
    }
    Rectangle {
        id: speedWrapper
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 64
        anchors.left: parent.left
        anchors.leftMargin: 64
        anchors.top: parent.top
        anchors.topMargin: 64
        color: "#ffffff"
        width: 300

        Text {
            id: speed
            anchors.fill: parent
            font.pixelSize: 64
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("0")
            verticalAlignment: Text.AlignVCenter
        }
    }
    Rectangle {
        id: rpmWrapper
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 64
        anchors.right: parent.right
        anchors.rightMargin: 64
        anchors.top: parent.top
        anchors.topMargin: 64
        color: "#ffffff"
        width: 300

        Text {
            id: rpm
            anchors.fill: parent
            font.pixelSize: 64
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("0")
            verticalAlignment: Text.AlignVCenter
        }
    }
}
