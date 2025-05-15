import QtQuick
import QtQuick.Controls

Item {
    id: root

    property string backgroundColor: "#3d3d3d"
    property string description
    property string fontColor: "#ffffff"
    property int fontSize: 148
    property int size: 300
    property int value: 0

    height: size
    width: size

    Rectangle {
        id: clusterWidget
        anchors.fill: parent
        color: backgroundColor
        radius: 32

        Text {
            id: valueBox
            anchors.bottom: clusterWidget.bottom
            anchors.bottomMargin: description ? 56 : 0
            anchors.left: clusterWidget.left
            anchors.leftMargin: 0
            anchors.right: clusterWidget.right
            anchors.rightMargin: 0
            anchors.top: clusterWidget.top
            anchors.topMargin: 0
            color: fontColor
            font.pixelSize: fontSize
            font.weight: 600
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            text: value
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            id: valueDescription
            anchors.bottom: clusterWidget.bottom
            anchors.bottomMargin: 0
            anchors.left: clusterWidget.left
            anchors.leftMargin: 0
            anchors.right: clusterWidget.right
            anchors.rightMargin: 0
            anchors.top: valueBox.bottom
            anchors.topMargin: -48
            color: fontColor
            font.pixelSize: 32
            horizontalAlignment: Text.AlignHCenter
            opacity: 1
            text: description
            verticalAlignment: Text.AlignTop
        }
    }
}
