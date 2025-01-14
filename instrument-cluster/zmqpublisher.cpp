#include "zmqpublisher.h"
#include <string>
#include <zmq.hpp>
#include <cstdint>
#include <iostream>

ZmqPublisher::ZmqPublisher(const std::string &socketAddress)
    : context(1), publisher(context, zmq::socket_type::pub) {
    try {
        publisher.bind(socketAddress);
    } catch (const zmq::error_t& e) {
        std::cerr << "Error trying socket connection: " << e.what() << std::endl;
    }
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

    try {
        publisher.send(message, zmq::send_flags::none);
    } catch (const zmq::error_t& e) {
        std::cerr << "Error trying to send message: " << e.what() << std::endl;
    }
}