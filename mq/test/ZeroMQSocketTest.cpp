#include <gtest/gtest.h>

#include <cstdint>
#include <vector>
#include <zmq.hpp>

#include "mq/src/ZeroMQSocket.hpp"

TEST(ZeroMQSocketTest, SendAndReceive) {
    zmq::context_t context(1);
    MQ::ZeroMQSocket sender(context, zmq::socket_type::push);
    MQ::ZeroMQSocket receiver(context, zmq::socket_type::pull);

    ASSERT_TRUE(sender.bind("inproc://test_socket"));
    ASSERT_TRUE(receiver.connect("inproc://test_socket"));

    std::vector<uint8_t> data{1, 2, 3};
    ASSERT_TRUE(sender.send(data));

    std::optional<std::vector<uint8_t>> received_message = receiver.receive();
    ASSERT_TRUE(received_message.has_value());

    ASSERT_EQ(data, received_message.value());

    sender.close();
    receiver.close();
}

TEST(ZeroMQSocketTest, ConnectFailure) {
    zmq::context_t context(1);
    MQ::ZeroMQSocket socket(context, zmq::socket_type::pub);
    ASSERT_FALSE(socket.connect("invalid_endpoint"));  // Expect connection failure
}

TEST(ZeroMQSocketTest, BindFailure) {
    zmq::context_t context(1);
    MQ::ZeroMQSocket socket1(context, zmq::socket_type::pub);
    MQ::ZeroMQSocket socket2(context, zmq::socket_type::pub);  // Bind to the same endpoint

    ASSERT_TRUE(socket1.bind("inproc://test_bind"));
    ASSERT_FALSE(socket2.bind("inproc://test_bind"));  // Should fail because already bound

    socket1.close();
}
