#include "ZMQSubscriber.hpp"

#include <QApplication>
#include <QtCore/QDebug>
#include <QtCore/QThread>
#include <csignal>
#include <cstdint>
#include <iostream>
#include <string>
#include <zmq.hpp>

ZmqSubscriber::ZmqSubscriber(const std::string &socketAddress, QObject *parent)
    : QObject(parent), context(1), subscriber(context, zmq::socket_type::sub) {
    qDebug() << "ZmqSubscriber::init";
    subscriber.connect(socketAddress);
    subscriber.set(zmq::sockopt::subscribe, "");
}

ZmqSubscriber::~ZmqSubscriber() {
    qDebug() << "ZmqSubscriber::closing socket";
    subscriber.close();
    qDebug() << "ZmqSubscriber::closing context";
    context.close();
}

int interrupted{0};

void signalHandler(int _) {
    (void)_;  // ignore unused variable
    interrupted = 1;
}

void catchSignals() {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    std::signal(SIGSEGV, signalHandler);
    std::signal(SIGABRT, signalHandler);
}

void ZmqSubscriber::checkForMessages() {
    qDebug() << "ZmqSubscriber::checkForMessages";
    zmq::message_t message;
    zmq::recv_result_t size;

    catchSignals();
    while (true) {
        try {
            size = subscriber.recv(message, zmq::recv_flags::dontwait);
        } catch (zmq::error_t &e) {
            qDebug() << "interrupt received";
        }

        if (interrupted) {
            qDebug() << "interrupt received, killing program...";
            break;
        }

        if (!size.has_value()) {
            continue;
        }
        decodeMessage(message);
    }
    qDebug() << "stopped receiving messages";
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
