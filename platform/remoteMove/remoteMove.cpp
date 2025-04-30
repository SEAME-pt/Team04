#include "remoteMove.hpp"
#include <cstring>
#include <stdexcept>
#include <chrono>

RemoteMove::RemoteMove(carMove& car_instance, const std::string& bind_address)
    : context(1),
      socket(context, zmq::socket_type::rep),
      address(bind_address),
      car(car_instance),
      running(false) {
    
    // Bind socket to address
    socket.bind(address);
    std::cout << "Remote control server started at " << address << std::endl;
}

RemoteMove::~RemoteMove() {
    stop();
}

void RemoteMove::start() {
    if (!running) {
        running = true;
        listener_thread = std::thread(&RemoteMove::listenerLoop, this);
        std::cout << "Remote control listener started" << std::endl;
    }
}

void RemoteMove::stop() {
    if (running) {
        running = false;
        if (listener_thread.joinable()) {
            listener_thread.join();
        }
        std::cout << "Remote control listener stopped" << std::endl;
    }
}

void RemoteMove::listenerLoop() {
    while (running) {
        zmq::message_t request;
        
        // Try to receive a message with timeout
        if (socket.recv(request, zmq::recv_flags::dontwait)) {
            // Check if the message size is correct (2 floats = 8 bytes)
            if (request.size() == 8) {
                auto* data = static_cast<float*>(request.data());
                float acceleration = data[0];
                float steering = data[1];
                
                // Ensure values are within the expected range
                acceleration = std::max(-1.0F, std::min(1.0F, acceleration));
                steering = std::max(-1.0F, std::min(1.0F, steering));
                
                // Process the command
                processCommand(acceleration, steering);
                
                // Send a success response
                zmq::message_t reply(2);
                memcpy(reply.data(), "OK", 2);
                socket.send(reply, zmq::send_flags::none);
            } else {
                // Invalid message format
                std::cout << "Received invalid message size: " << request.size() << " bytes" << std::endl;
                
                // Send an error response
                zmq::message_t reply(5);
                memcpy(reply.data(), "ERROR", 5);
                socket.send(reply, zmq::send_flags::none);
            }
        }
        
        // Small sleep to prevent CPU hogging
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void RemoteMove::processCommand(float acceleration, float steering) {
    // Convert acceleration (-1 to 1) to motor speed (-100 to 100)
    int motorSpeed = static_cast<int>(acceleration * 100);
    
    // Convert steering (-1 to 1) to servo angle (-MAX_ANGLE to MAX_ANGLE)
    // Note: We're using the MAX_ANGLE constant from carMove (which is 80)
    // but we'll limit it to 45 degrees as that seems to be the effective range
    int servoAngle = static_cast<int>(steering * 45);
    
    std::cout << "Command received - Acceleration: " << acceleration
              << " (" << motorSpeed << "%), Steering: " << steering
              << " (" << servoAngle << "°)" << std::endl;
    
    // Apply the commands to the car
    car.setMotorSpeed(motorSpeed);
    car.setServoAngle(servoAngle);
}