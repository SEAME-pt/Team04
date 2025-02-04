pragma Singleton
import QtQuick
import DataManager 1.0

Item {
    id: backend

    property SequentialAnimation __anim01: SequentialAnimation {
        loops: -1
        running: true

        PropertyAnimation {
            duration: 5000
            from: 0.3
            property: "speed"
            target: backend
            to: 14.6
        }
        PropertyAnimation {
            duration: 5000
            from: 14.6
            property: "speed"
            target: backend
            to: 0.3
        }
    }
    property SequentialAnimation __anim02: SequentialAnimation {
        loops: -1
        running: true

        PropertyAnimation {
            duration: 50000
            from: 100
            property: "battery"
            target: backend
            to: 14
        }
        PropertyAnimation {
            duration: 50000
            from: 14
            property: "battery"
            target: backend
            to: 100
        }
    }
    property SequentialAnimation __anim03: SequentialAnimation {
        loops: -1
        running: true

        PropertyAnimation {
            duration: 5000
            from: 60
            property: "temperature"
            target: backend
            to: 100
        }
        PropertyAnimation {
            duration: 5000
            from: 100
            property: "temperature"
            target: backend
            to: 60
        }
    }
    property SequentialAnimation __anim04: SequentialAnimation {
        loops: -1
        running: true

        PropertyAnimation {
            duration: 5000
            from: 254
            property: "rpm"
            target: backend
            to: 956
        }
        PropertyAnimation {
            duration: 5000
            from: 956
            property: "rpm"
            target: backend
            to: 254
        }
    }
    property date __currentTime: new Date()
    property real battery: 0
    property int hours: 0
    property bool metricSystem: true
    property int minutes: 0
    readonly property real ratioKMtoM: 0.62137
    property real rpm: 0
    property int seconds: 0
    property real speed: 0
    property real speedComputed: backend.metricSystem ? (backend.speed * backend.ratioKMtoM) : backend.speed
    property real temperature: 0
    property string time: ""

    signal reset

    function __timeUpdate() {
        seconds = backend.__currentTime.getSeconds();
        minutes = backend.__currentTime.getMinutes();
        hours = backend.__currentTime.getHours();
        time = backend.__currentTime.toLocaleTimeString({
                "hour": '2-digit',
                "minute": '2-digit'
            });
    }
    function resetAll() {
        resetSettings();
        backend.speed = 10;
        backend.battery = 50;
        __anim01.restart();
        __anim02.restart();
        __anim03.restart();
        __anim04.restart();
        __currentTime = new Date();
        reset();
    }
    function resetSettings() {
        backend.metricSystem = true;
    }

    Component.onCompleted: {
        __timeUpdate();
    }

    DataManager {
        id: dataManager
    }
    Timer {
        interval: 1000
        repeat: true
        running: true

        onTriggered: {
            backend.__currentTime = new Date();
            __timeUpdate();
        }
    }
}
