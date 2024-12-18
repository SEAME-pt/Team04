#ifndef ZMQPUBLISHER_H
#define ZMQPUBLISHER_H

#include <zmq.hpp>
#include <string>

class ZmqPublisher {
   public:
    explicit ZmqPublisher(const std::string &socketAddress);
    ~ZmqPublisher();

    void sendMessage(uint8_t speed, uint16_t rpm);

   private:
    zmq::context_t context;
    zmq::socket_t publisher;
};

#endif // ZMQPUBLISHER_H
