#include "../zmqsubscriber.h"
#include "qobject.h"
#include <gtest/gtest.h>
#include <QObject>
#include <cstdint>
#include <zmq.hpp>

class TestZmqSubscriber : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(TestZmqSubscriber, DecodeMessageTest) {
    ZmqSubscriber subscriber("ipc://test");

    uint8_t receivedSpeed = 0;
    uint16_t receivedRpm = 0;

    QObject::connect(&subscriber, &ZmqSubscriber::messageReceived,
                     [&](uint8_t speed, uint16_t rpm) {
                         receivedSpeed = speed;
                         receivedRpm = rpm;
                     });

    uint8_t messageData[4] = {100, 0, 0x70, 0x17}; // Speed = 100, RPM = 6000
    zmq::message_t message(messageData, 4);

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PUB);
    socket.connect("ipc://test");

    socket.send(message, zmq::send_flags::none);

    subscriber.checkForMessages(); 

    EXPECT_EQ(receivedSpeed, 100);
    EXPECT_EQ(receivedRpm, 6000);
}

TEST_F(TestZmqSubscriber, InvalidMessageTest) {
    ZmqSubscriber subscriber("ipc://test");

    bool signalTriggered = false;

    QObject::connect(&subscriber, &ZmqSubscriber::messageReceived,
                     [&](uint8_t, uint16_t) {
                         signalTriggered = true;
                     });

    uint8_t invalidData[2] = {50, 0}; 
    zmq::message_t invalidMessage(invalidData, 2);

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PUB);
    socket.connect("ipc://test");

    socket.send(invalidMessage, zmq::send_flags::none);

    subscriber.checkForMessages(); 

    EXPECT_FALSE(signalTriggered);
}

TEST_F(TestZmqSubscriber, InvalidMessageContentTest) {
    ZmqSubscriber subscriber("ipc://test");

    bool signalTriggered = false;

    QObject::connect(&subscriber, &ZmqSubscriber::messageReceived,
                     [&](uint8_t, uint16_t) {
                         signalTriggered = true;
                     });

    uint8_t invalidData[4] = {100, 0, 0x70, 0x00}; 
    zmq::message_t invalidMessage(invalidData, 4);

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PUB);
    socket.connect("ipc://test");

    socket.send(invalidMessage, zmq::send_flags::none);

    subscriber.checkForMessages();

    EXPECT_FALSE(signalTriggered);
}

TEST_F(TestZmqSubscriber, MultipleMessagesTest) {
    ZmqSubscriber subscriber("ipc://test");

    int messageCount = 0;

    QObject::connect(&subscriber, &ZmqSubscriber::messageReceived,
                     [&](uint8_t, uint16_t) {
                         messageCount++;
                     });

    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PUB);
    socket.connect("ipc://test");

    for (int i = 0; i < 5; ++i) {
        uint8_t messageData[4] = {static_cast<uint8_t>(i * 20), 0, 0x70, 0x17};
        zmq::message_t message(messageData, 4);
        
        socket.send(message, zmq::send_flags::none);
    }

    subscriber.checkForMessages(); 

    EXPECT_EQ(messageCount, 5);
}