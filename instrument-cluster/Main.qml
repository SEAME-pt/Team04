import QtQuick
import QtQuick.Controls

ApplicationWindow {
    height: mainScreen.height
    title: qsTr("Instrument Cluster")
    visible: true
    width: mainScreen.width

    ScreenUI {
        id: mainScreen
    }
}
