#include "joystick.hpp"

joystick::joystick(carMove& car) : gameController(nullptr), remote(nullptr), car_ref(&car) {
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        throw std::runtime_error("Failed to initialize SDL2 GameController: " +
                                 std::string(SDL_GetError()));
    }
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
}

joystick::~joystick() {
    delete remote;
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
    setAxisAction(0, [&car](int value) {
        float fvalue = value * 1.0;
        fvalue = fvalue / 32000.0 * 55;
        car.setServoAngle(fvalue);
    });

    setAxisAction(5, [&car](int value) {
        value -= 16319;
        value = (value / 165) * -1;
        car.setMotorSpeed(value);
    });
}

void joystick::processEvent(const SDL_Event& event) {
    if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP) {
        bool is_pressed = (event.type == SDL_CONTROLLERBUTTONDOWN);
        int button = event.cbutton.button;

        // std::cout << "Button " << button << " " << (is_pressed ? "PRESSED" : "RELEASED") <<
        // std::endl;

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
        std::cout << "Game controller disconnected." << '\n';

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
        // Original exit conditions
        if (buttonStates[4] && buttonStates[6]) {
            if (car_ref) {
                car_ref->setMotorSpeed(0);
                car_ref->setServoAngle(0);
            }
            if (remote) {
                remote->stop();
                delete remote;
                remote = nullptr;
            }
            if (gameController) {
                SDL_GameControllerClose(gameController);
                gameController = nullptr;
            }
            SDL_Quit();
            std::cout << "All resources cleaned up. Exiting.\n";
            std::exit(EXIT_SUCCESS);
        }
        if (gameController == nullptr) {
            std::cout << "No controller connected, stopping listen.\n";
            break;
        }
        SDL_Delay(10);
    }
}

void joystick::printButtonStates() {
    std::cout << "======= CONTROLLER BUTTON SETUP =======" << '\n';
    std::cout << "Button 1: Autonomous driving ON" << '\n';
    std::cout << "Button 3: Autonomous driving OFF" << '\n';
    std::cout << "Buttons 4+6 together: Exit program" << '\n';
    std::cout << "=====================================" << '\n';

    std::cout << "Current button states:" << '\n';
    for (size_t i = 0; i < buttonStates.size(); ++i) {
        if (buttonStates[i]) {
            std::cout << "Button " << i << ": PRESSED" << '\n';
        }
    }
}
