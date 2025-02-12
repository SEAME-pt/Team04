import QtQuick
import QtQuick.Controls
import Constants

ApplicationWindow {
    height: Constants.height
    title: qsTr("Instrument Cluster")
    visibility: SIMULATION_MODE ? "AutomaticVisibility" : "FullScreen"
    visible: true
    width: Constants.width

    ScreenUI {
        id: mainScreen
    }
}
