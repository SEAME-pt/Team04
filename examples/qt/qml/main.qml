import QtQuick 6.0
import QtQuick.Controls 6.0

ApplicationWindow {

    // Define properties
    property color bgColor: "lightblue"
    property int fontSize: 24

    height: 480
    title: "Qt6 QML Example"
    visible: true
    width: 640

    Rectangle {
        color: bgColor
        height: parent.height
        width: parent.width

        // Text element that changes based on interaction
        Text {
            id: label
            anchors.centerIn: parent
            color: "black"
            font.pixelSize: fontSize
            text: "Adjust me!"
        }

        // Slider to adjust font size
        Slider {
            id: fontSizeSlider
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: label.bottom
            from: 10
            to: 100
            value: fontSize
            width: parent.width * 0.8

            onValueChanged: {
                fontSize = fontSizeSlider.value;
            }
        }

        // CheckBox to toggle background color
        CheckBox {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: fontSizeSlider.bottom
            text: "Change Background Color"

            onCheckedChanged: {
                bgColor = checked ? "lightgreen" : "lightblue";
            }
        }

        // Button to reset the font size and background color
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: checkBox.bottom
            text: "Reset"

            onClicked: {
                fontSize = 24;
                bgColor = "lightblue";
                fontSizeSlider.value = 24;
                checkBox.checked = false;
            }
        }
    }
}
