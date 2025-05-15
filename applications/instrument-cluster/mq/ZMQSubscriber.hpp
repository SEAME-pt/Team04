#ifndef ZMQSUBSCRIBER_H
#define ZMQSUBSCRIBER_H

#include <QtCore/QObject>
#include <zmq.hpp>

#include "mq/src/ZeroMQSocket.hpp"

class ZmqSubscriber : public QObject {
    // NOLINTNEXTLINE(modernize-use-trailing-return-type)
    Q_OBJECT

   public:
    explicit ZmqSubscriber(std::unique_ptr<MQ::ZeroMQSocket> socket, QObject *parent = nullptr);
    ~ZmqSubscriber() override;

   signals:
    void messageReceived(float speed, float rpm);
    void batteryMessageReceived(uint8_t battery);
    void temperatureMessageReceived(uint8_t temperature);

   public slots:
    void checkForMessages();

   private:
    void decodeMessage(const std::vector<uint8_t> &message);
    std::unique_ptr<MQ::ZeroMQSocket> subscriber;
};

#endif  // ZMQSUBSCRIBER_H
