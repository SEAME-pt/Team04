#include "joystick.hpp"

joystick::joystick(carMove& car) : gameController(nullptr), remote(nullptr) {
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        throw std::runtime_error("Failed to initialize SDL2 GameController: " +
                                 std::string(SDL_GetError()));
    }

    // Create the RemoteMove object as a member variable
    remote = new RemoteMove(car, "tcp://*:5555");

    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i) != 0U) {
            gameController = SDL_GameControllerOpen(i);
            if (gameController != nullptr) {
                setAxisMapping(car);
                std::cout << "Game controller connected: " << SDL_GameControllerName(gameController)
                          << '\n';
                setButtonAction(1, Actions{[this, &car]() {
                                               std::cout << "Autonomous driving on\n";
                                               if (remote) {
                                                   remote->start();
                                               }
                                           },
                                           []() {}});

                setButtonAction(3, Actions{[this, &car]() {
                                               std::cout << "Autonomous driving out\n";
                                               if (remote) {
                                                   remote->stop();
                                               }
                                           },
                                           [&car]() {
                                               std::cout
                                                   << "Button 3 released - Returning to neutral\n";
                                               car.setServoAngle(0);
                                           }});

                printButtonStates();
                break;
            }
        }
    }
    // Rest of your constructor
}

joystick::~joystick() {
    delete remote;  // Clean up the RemoteMove object
    if (gameController != nullptr) {
        SDL_GameControllerClose(gameController);
        gameController = nullptr;
    }
    SDL_Quit();
}

void joystick::setButtonAction(int button, Actions actions) {
    buttonActions[button] = std::move(actions);
}

void joystick::setAxisAction(int axis, std::function<void(int)> action) {
    axisActions[axis] = std::move(action);
}

void joystick::setAxisMapping(carMove& car) {
    // Left stick X-axis controls the servo (steering)
    setAxisAction(0, [&car](int value) {
        float fvalue = value * 1.0;  // Raw value

        // Normalize to a 45-degree range
        fvalue = fvalue / 32000.0 * 55;
        car.setServoAngle(fvalue);
    });

    // Right stick Y-axis controls the motor speed
    setAxisAction(5, [&car](int value) {
        // Normalize SDL axis value (-32768 to 32767) to speed percentage (-100% to 100%)
        value -= 16319;  // Adjust for neutral position
        value = (value / 165) * -1;
        car.setMotorSpeed(value);  // Invert to match joystick forward/backward direction
    });
}

void joystick::processEvent(const SDL_Event& event) {
    if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP) {
        bool is_pressed = (event.type == SDL_CONTROLLERBUTTONDOWN);
        int button = event.cbutton.button;

        if (static_cast<std::size_t>(button) < buttonStates.size()) {
            buttonStates[button] = is_pressed;
            if (buttonActions.find(button) != buttonActions.end()) {
                if (is_pressed) {
                    buttonActions[button].onPress();
                } else {
                    buttonActions[button].onRelease();
                }
            }
        }
    } else if (event.type == SDL_CONTROLLERAXISMOTION) {
        int axis = event.caxis.axis;
        int value = event.caxis.value;

        if (axisActions.find(axis) != axisActions.end()) {
            axisActions[axis](value);
        }
    } else if (event.type == SDL_CONTROLLERDEVICEREMOVED) {
        // Handle the disconnection of the game controller
        std::cout << "Game controller disconnected." << '\n';

        // Close the controller and set to nullptr
        if (gameController != nullptr) {
            SDL_GameControllerClose(gameController);
            gameController = nullptr;
        }

        std::cout << "Exiting the program due to controller disconnection." << '\n';
        exit(1);
    }
}

void joystick::listen() {
    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event) != 0) {
            processEvent(event);
        }
        if (buttonStates[4] && buttonStates[6]) {
            break;
        }
        // If no controller is connected, break the loop or handle accordingly
        if (gameController == nullptr) {
            std::cout << "No controller connected, stopping listen." << '\n';
            break;
        }

        if (gameController == nullptr) {
            std::cout << "No controller connected, stopping listen." << '\n';
            break;
        }
        SDL_Delay(10);
    }
}

void joystick::printButtonStates() {
    setButtonAction(0, Actions{[]() {}, []() {}});
    setButtonAction(1, Actions{[]() {}, []() {}});
    setButtonAction(2, Actions{[]() {}, []() {}});
    setButtonAction(3, Actions{[]() {}, []() {}});
    setButtonAction(4, Actions{[]() {}, []() {}});
    setButtonAction(5, Actions{[]() {}, []() {}});
    setButtonAction(6, Actions{[]() {}, []() {}});
    setButtonAction(7, Actions{[]() {}, []() {}});
    setButtonAction(8, Actions{[]() {}, []() {}});
    setButtonAction(9, Actions{[]() {}, []() {}});
    setButtonAction(10, Actions{[]() {}, []() {}});
    setButtonAction(11, Actions{[]() {}, []() {}});
    setButtonAction(12, Actions{[]() {}, []() {}});
    setButtonAction(13, Actions{[]() {}, []() {}});
    setButtonAction(14, Actions{[]() {}, []() {}});
    setButtonAction(15, Actions{[]() {}, []() {}});
    setButtonAction(16, Actions{[]() {}, []() {}});
}
