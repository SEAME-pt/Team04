#ifndef REMOTE_MOVE_HPP
#define REMOTE_MOVE_HPP

#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include <zmq.hpp>

#include "../carMove/carMove.hpp"

class RemoteMove {
   public:
    /**
     * Constructor
     * @param car_instance Reference to a carMove object that will be controlled
     * @param bind_address ZeroMQ socket address to bind to (e.g., "tcp://0.0.0.0:5555")
     */
    explicit RemoteMove(carMove& car_instance, const std::string& bind_address = "tcp://*:5555");

    /**
     * Destructor - stops the listener thread if running
     */
    ~RemoteMove();

    /**
     * Start the listener thread to receive commands
     */
    void start();

    /**
     * Stop the listener thread
     */
    void stop();

   private:
    // ZeroMQ context and socket
    zmq::context_t context;
    zmq::socket_t socket;
    std::string address;

    // Car control object
    carMove& car;

    // Thread management
    std::atomic<bool> running;
    std::thread listener_thread;

    // Listener method that runs in a separate thread
    void listenerLoop();

    // Process incoming command
    void processCommand(float acceleration, float steering);
};

#endif  // REMOTE_MOVE_HPP
