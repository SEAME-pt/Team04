#include "basicdisplay.h"
#include <zmq.hpp>
#include <QApplication>

void decodeMessage(const zmq::message_t &message, BasicDisplay *display) {
    const uint8_t* data = static_cast<const uint8_t*>(message.data());

    uint8_t speedValue = data[0];
    uint8_t unit = data[1];
    uint8_t rpmLowByte = data[2];
    uint8_t rpmHighByte = data[3];

    uint16_t rpm = (rpmHighByte << 8) | rpmLowByte;

    std::string unitString = (unit == 0x00) ? "km/h" : "mph";

    display->updateSpeed(speedValue);
    display->updateRPM(rpm);
    display->show();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    zmq::context_t context(1);

    zmq::socket_t subscriber(context, zmq::socket_type::sub);

    subscriber.connect("tcp://localhost:5556");

    subscriber.setsockopt(ZMQ_SUBSCRIBE, "speed", 5);

    BasicDisplay display;

    while (true) {
        zmq::message_t message;
        subscriber.recv(message, zmq::recv_flags::none);

        std::string received_msg(static_cast<char*>(message.data()), message.size());

        decodeMessage(message, &display);
    }

    return a.exec();
}
