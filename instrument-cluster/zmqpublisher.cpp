#include "zmqpublisher.h"

ZmqPublisher::ZmqPublisher(const std::string &socketAddress)
    : context(1), publisher(context, zmq::socket_type::pub) {
    publisher.bind(socketAddress);
}

ZmqPublisher::~ZmqPublisher() {
    publisher.close();
}

void ZmqPublisher::sendMessage(uint8_t speed, uint16_t rpm) {
    uint8_t messageData[4];
    messageData[0] = speed;
    messageData[1] = 0;
    messageData[2] = rpm & 0xFF;
    messageData[3] = (rpm >> 8) & 0xFF;

    zmq::message_t message(messageData, 4);
    publisher.send(message, zmq::send_flags::none);
}
