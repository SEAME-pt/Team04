#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mq/src/IMQSocket.hpp"
#include "mq/src/ZeroMQWorker.hpp"

using ::testing::Return;

class MockMQSocket : public MQ::IMQSocket {  // Create a Mock Class
   public:
    // NOLINTNEXTLINE(modernize-use-trailing-return-type)
    MOCK_METHOD(bool, bind, (const std::string &), (override));
    // NOLINTNEXTLINE(modernize-use-trailing-return-type)
    MOCK_METHOD(bool, connect, (const std::string &), (override));
    // NOLINTNEXTLINE(modernize-use-trailing-return-type)
    MOCK_METHOD(bool, subscribe, (const std::string &), (override));
    // NOLINTNEXTLINE(modernize-use-trailing-return-type)
    MOCK_METHOD(bool, send, (const std::vector<uint8_t> &), (override));
    // NOLINTNEXTLINE(modernize-use-trailing-return-type)
    MOCK_METHOD(std::optional<std::vector<uint8_t>>, receive, (), (override));
    MOCK_METHOD(void, close, (), (override));
};

void testSendPublisher(bool sendReturn) {
    auto mock = std::make_unique<MockMQSocket>();

    std::vector<uint8_t> test_argument{1, 2, 3};
    // mocking methods
    EXPECT_CALL(*mock, bind(testing::_)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mock, close()).Times(1);
    EXPECT_CALL(*mock, send(test_argument)).Times(1).WillOnce(Return(sendReturn));

    MQ::ZeroMQWorker publisher{std::move(mock)};
    publisher.subscribe("mock");
    EXPECT_EQ(publisher.publish(test_argument), sendReturn);
}

TEST(ZeroMQTest, PublisherSendTestSuccessReturnsTrue) { testSendPublisher(true); }

TEST(ZeroMQTest, PublisherSendTestFailReturnFalse) { testSendPublisher(false); }

void testSendSubscriber(std::optional<std::vector<uint8_t>> result) {
    auto mock = std::make_unique<MockMQSocket>();

    // mocking methods
    EXPECT_CALL(*mock, bind(testing::_)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*mock, close()).Times(1);
    EXPECT_CALL(*mock, receive()).Times(1).WillOnce(Return(result));

    MQ::ZeroMQWorker subscriber{std::move(mock)};
    subscriber.subscribe("mock");
    EXPECT_EQ(subscriber.receive(), result);
}

TEST(ZeroMQTest, SubscriberReceiveTestSuccessReturnsVector) {
    std::optional<std::vector<uint8_t>> expected_result =
        std::make_optional<std::vector<uint8_t>>({1, 2, 3});
    testSendSubscriber(expected_result);
}

TEST(ZeroMQTest, SubscriberReceiveTestFailReturnsEmpty) { testSendSubscriber({}); }
