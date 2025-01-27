#include "ZMQSubscriber.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <zmq.hpp>

ZmqSubscriber::ZmqSubscriber(const std::string &socketAddress, QObject *parent)
    : QObject(parent), context(1), subscriber(context, zmq::socket_type::sub) {
    subscriber.connect(socketAddress);
    subscriber.set(zmq::sockopt::subscribe, "");
}

ZmqSubscriber::~ZmqSubscriber() { subscriber.close(); }

void ZmqSubscriber::checkForMessages() {
    zmq::message_t message;
    while (subscriber.recv(message, zmq::recv_flags::dontwait)) {
        decodeMessage(message);
    }
}

void ZmqSubscriber::decodeMessage(const zmq::message_t &message) {
    if (message.size() < 4) {
        std::cerr << "Error: Invalid message - size is too small.\n";
        return;
    }

    const auto *data = static_cast<const uint8_t *>(message.data());

    uint8_t const speed_value = data[0];
    uint8_t const rpm_low_byte = data[2];
    uint8_t const rpm_high_byte = data[3];

    uint16_t const rpm = (rpm_high_byte << 8) | rpm_low_byte;

    emit messageReceived(speed_value, rpm);
}
