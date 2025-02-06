#include "ZMQSubscriber.hpp"

#include <QApplication>
#include <QtCore/QDebug>
#include <QtCore/QThread>
#include <csignal>
#include <cstdint>
#include <iostream>
#include <zmq.hpp>

ZmqSubscriber::ZmqSubscriber(std::unique_ptr<MQ::ZeroMQSocket> socket, QObject *parent)
    : QObject(parent), subscriber(std::move(socket)) {
    qDebug() << "ZmqSubscriber::init";
}

ZmqSubscriber::~ZmqSubscriber() {
    qDebug() << "ZmqSubscriber::closing socket";
    subscriber->close();
    qDebug() << "ZmqSubscriber::finished";
}

void ZmqSubscriber::checkForMessages() {
    qDebug() << "ZmqSubscriber::checkForMessages";
    subscriber->subscribe("");
    std::optional<std::vector<uint8_t>> message;

    while (true) {
        try {
            message = subscriber->receive();
        } catch (zmq::error_t &e) {
            qDebug() << e.what();
            if (ETERM == e.num()) {
                break;
            }
        }

        if (!message.has_value()) {
            continue;
        }
        decodeMessage(message.value());
    }
    qDebug() << "stopped receiving messages";
}

void ZmqSubscriber::decodeMessage(const std::vector<uint8_t> &message) {
    if (message.empty()) {
        std::cerr << "Error: Invalid message - size is too small.\n";
        return;
    }

    switch (message[0]) {
        case 1: {
            uint8_t const speed_value = message[1];
            uint8_t const rpm_low_byte = message[3];
            uint8_t const rpm_high_byte = message[4];
            uint16_t const rpm = (rpm_high_byte << 8) | rpm_low_byte;
            emit messageReceived(speed_value, rpm);
            break;
        }
        case 2: {
            uint8_t const battery = message[1];
            emit batteryMessageReceived(battery);
            break;
        }
        case 3: {
            uint8_t const temperature = message[1];
            emit temperatureMessageReceived(temperature);
            break;
        }
        default:
            qDebug() << "No message definition";
    }
}
