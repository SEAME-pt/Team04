#include "../zmqsubscriber.h"
#include <gtest/gtest.h>
#include <QObject>

class TestZmqSubscriber : public ::testing::Test {
   protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(TestZmqSubscriber, DecodeMessageTest) {
    ZmqSubscriber subscriber("ipc://test");

    uint8_t receivedSpeed = 0;
    uint16_t receivedRpm = 0;

    QObject::connect(&subscriber, &ZmqSubscriber::messageReceived, [&](uint8_t speed, uint16_t rpm) {
        receivedSpeed = speed;
        receivedRpm = rpm;
    });

            // Simular uma mensagem válida
    uint8_t messageData[4] = {100, 0, 0x70, 0x17}; // Speed = 100, RPM = 6000
    zmq::message_t message(messageData, 4);

    subscriber.checkForMessages(); // Força o processamento da mensagem

    EXPECT_EQ(receivedSpeed, 100);
    EXPECT_EQ(receivedRpm, 6000);
}

TEST_F(TestZmqSubscriber, InvalidMessageTest) {
    ZmqSubscriber subscriber("ipc://test");

    bool signalTriggered = false;

    QObject::connect(&subscriber, &ZmqSubscriber::messageReceived, [&](uint8_t, uint16_t) {
        signalTriggered = true;
    });

    uint8_t invalidData[2] = {50, 0}; // Mensagem inválida
    zmq::message_t invalidMessage(invalidData, 2);

    subscriber.checkForMessages(); // Não deve emitir sinal

    EXPECT_FALSE(signalTriggered);
}
