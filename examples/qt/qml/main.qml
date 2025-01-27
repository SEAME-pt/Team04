import QtQuick 6.0
import QtQuick.Controls 6.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "Qt6 QML Example"

    // Define properties
    property color bgColor: "lightblue"
    property int fontSize: 24

    Rectangle {
        width: parent.width
        height: parent.height
        color: bgColor

        // Text element that changes based on interaction
        Text {
            id: label
            text: "Adjust me!"
            anchors.centerIn: parent
            font.pixelSize: fontSize
            color: "black"
        }

        // Slider to adjust font size
        Slider {
            id: fontSizeSlider
            from: 10
            to: 100
            value: fontSize
            anchors.top: label.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width * 0.8
            onValueChanged: {
                fontSize = fontSizeSlider.value
            }
        }

        // CheckBox to toggle background color
        CheckBox {
            text: "Change Background Color"
            anchors.top: fontSizeSlider.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            onCheckedChanged: {
                bgColor = checked ? "lightgreen" : "lightblue"
            }
        }

        // Button to reset the font size and background color
        Button {
            text: "Reset"
            anchors.top: checkBox.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                fontSize = 24
                bgColor = "lightblue"
                fontSizeSlider.value = 24
                checkBox.checked = false
            }
        }
    }
}
