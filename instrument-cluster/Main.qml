import QtQuick
import QtQuick.Controls
import Constants

ApplicationWindow {
    height: Constants.height
    title: qsTr("Instrument Cluster")
    visible: true
    width: Constants.width

    ScreenUI {
        id: mainScreen
    }
}
