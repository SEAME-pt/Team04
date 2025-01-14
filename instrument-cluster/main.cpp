#include "basicdisplay.h"
#include "qapplication.h"
#include "qobject.h"
#include "qtimer.h"
#include "zmqsubscriber.h"
#include "config.h"
#include <QApplication>
#include <QTimer>
#include <cstdint>

auto main(int argc, char *argv[]) -> int {
    QApplication const app(argc, argv);

    BasicDisplay display;

    ZmqSubscriber const subscriber(ZMQ_SOCKET_ADDRESS);

    QObject::connect(&subscriber, &ZmqSubscriber::messageReceived, [&](uint8_t speed, uint16_t rpm) {
        display.updateSpeed(speed);
        display.updateRPM(rpm);
    });

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, &subscriber, &ZmqSubscriber::checkForMessages);
    timer.start(TIMER_INTERVAL_MS);

    display.show();
    return QApplication::exec();
}

