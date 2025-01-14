#include "zmqsubscriber.h"
#include <string>
#include "qobject.h"
#include <zmq.hpp>
#include <cstdint>
#include "qobjectdefs.h"

ZmqSubscriber::ZmqSubscriber(const std::string &socketAddress, QObject *parent)
    : QObject(parent), context(1), subscriber(context, zmq::socket_type::sub) {
    subscriber.connect(socketAddress);
    subscriber.set(zmq::sockopt::subscribe, "");
}

ZmqSubscriber::~ZmqSubscriber() {
    subscriber.close();
}

void ZmqSubscriber::checkForMessages() {
    zmq::message_t message;
    if (subscriber.recv(message, zmq::recv_flags::dontwait)) {
        decodeMessage(message);
    }
}

void ZmqSubscriber::decodeMessage(const zmq::message_t &message) {
    if (message.size() < 4) {
        return;
    }

    const auto* data = static_cast<const uint8_t*>(message.data());

    uint8_t const speedValue = data[0];
    uint8_t const rpmLowByte = data[2];
    uint8_t const rpmHighByte = data[3];

    uint16_t const rpm = (rpmHighByte << 8) | rpmLowByte;

    emit messageReceived(speedValue, rpm);
}
