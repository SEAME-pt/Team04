#ifndef ZMQSUBSCRIBER_H
#define ZMQSUBSCRIBER_H

#include <zmq.hpp>
#include <QObject>

class ZmqSubscriber : public QObject {
    Q_OBJECT

   public:
    explicit ZmqSubscriber(const std::string &socketAddress, QObject *parent = nullptr);
    ~ZmqSubscriber();

   signals:
    void messageReceived(uint8_t speed, uint16_t rpm);

   public slots:
    void checkForMessages();

   private:
    void decodeMessage(const zmq::message_t &message);

    zmq::context_t context;
    zmq::socket_t subscriber;
};

#endif // ZMQSUBSCRIBER_H
