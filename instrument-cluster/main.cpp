#include "basicdisplay.h"
#include "zmqsubscriber.h"
#include "config.h"
#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    BasicDisplay display;

    ZmqSubscriber subscriber(ZMQ_SOCKET_ADDRESS);

    QObject::connect(&subscriber, &ZmqSubscriber::messageReceived, [&](uint8_t speed, uint16_t rpm) {
        display.updateSpeed(speed);
        display.updateRPM(rpm);
    });

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, &subscriber, &ZmqSubscriber::checkForMessages);
    timer.start(TIMER_INTERVAL_MS);

    display.show();
    return app.exec();
}
