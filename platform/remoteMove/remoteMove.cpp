#include "remoteMove.hpp"

#include <chrono>
#include <cstring>
#include <stdexcept>

RemoteMove::RemoteMove(CarMove& car_instance, std::string bind_address)
    : context(1),
      socket(context, zmq::socket_type::rep),
      address(std::move(bind_address)),
      car(car_instance),
      running(false) {
    socket.bind(address);
    std::cout << "Remote control server started at " << address << "\n";
}

RemoteMove::~RemoteMove() { stop(); }

void RemoteMove::start() {
    if (!running) {
        running = true;
        listener_thread = std::thread(&RemoteMove::listenerLoop, this);
        std::cout << "Remote control listener started" << "\n";
    }
}

void RemoteMove::stop() {
    if (running) {
        running = false;
        if (listener_thread.joinable()) {
            listener_thread.join();
        }
        std::cout << "Remote control listener stopped" << "\n";
    }
}

void RemoteMove::listenerLoop() {
    while (running) {
        zmq::message_t request;
        if (socket.recv(request, zmq::recv_flags::dontwait)) {
            if (request.size() == 8) {
                auto* data = static_cast<float*>(request.data());
                float acceleration = data[0];
                float steering = data[1];
                acceleration = std::max(-1.0F, std::min(1.0F, acceleration));
                steering = std::max(-1.0F, std::min(1.0F, steering));
                processCommand(acceleration, steering);
                zmq::message_t reply(2);
                memcpy(reply.data(), "OK", 2);
                socket.send(reply, zmq::send_flags::none);
            } else {
                std::cout << "Received invalid message size: " << request.size() << " bytes"
                          << "\n";
                zmq::message_t reply(5);
                memcpy(reply.data(), "ERROR", 5);
                socket.send(reply, zmq::send_flags::none);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void RemoteMove::processCommand(float acceleration, float steering) {
    // Convert acceleration (-1 to 1) to motor speed (-100 to 100)
    int motorspeed = static_cast<int>(acceleration * 100);

    // Convert steering (-1 to 1) to servo angle (-MAX_ANGLE to MAX_ANGLE)
    int servo_angle = static_cast<int>(steering * 45);
    std::cout << "Command received - Acceleration: " << acceleration << " (" << motorspeed
              << "%), Steering: " << steering << " (" << servo_angle << "°)!n";
    // Apply the commands to the car
    car.setMotorSpeed(motorspeed);
    car.setServoAngle(servo_angle);
}
