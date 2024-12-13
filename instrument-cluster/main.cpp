#include "basicdisplay.h"
#include <iostream>
#include <zmq.hpp>
#include <QTimer>
#include <QApplication>

void decodeMessage(const zmq::message_t &message, BasicDisplay *display) {
    if (message.size() < 4) {
        // Mensagem inválida, não processar
        return;
    }

    const uint8_t* data = static_cast<const uint8_t*>(message.data());

    uint8_t speedValue = data[0];
    uint8_t unit = data[1];
    uint8_t rpmLowByte = data[2];
    uint8_t rpmHighByte = data[3];

    uint16_t rpm = (rpmHighByte << 8) | rpmLowByte;

    display->updateSpeed(speedValue);
    display->updateRPM(rpm);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    zmq::context_t context(1);

    zmq::socket_t subscriber (context, zmq::socket_type::sub);

    try {
        subscriber.connect("tcp://localhost:5556");
        subscriber.connect("ipc:///tmp/speed.ipc");
    } catch (const zmq::error_t& e) {
        std::cerr << "Erro trying to connect: " << e.what() << std::endl;
        return -1;
    }

    subscriber.set(zmq::sockopt::subscribe, ""); //Subscribe to all messages

    BasicDisplay display;

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        zmq::message_t message;
        if (subscriber.recv(message, zmq::recv_flags::none)) {
            decodeMessage(message, &display);
        }
    });
    timer.start(100); // Check for messages every 100 ms.

    QObject::connect(&a, &QApplication::aboutToQuit, [&]() {
        subscriber.close();
        context.close();
    });

    display.show();
    return a.exec();
}
