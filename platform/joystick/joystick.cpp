#include "joystick.hpp"

#include <utility>

joystick::joystick(carMove& car) : gameController(nullptr) {
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        throw std::runtime_error("Failed to initialize SDL2 GameController: " +
                                 std::string(SDL_GetError()));
    }

    // Abrir o primeiro controlador de jogo disponível
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i) != 0U) {
            gameController = SDL_GameControllerOpen(i);
            if (gameController != nullptr) {
                std::cout << "GameController connected: " << SDL_GameControllerName(gameController)
                          << '\n';
                setPS4AxisMapping(car);
                break;
            }
        }
    }

    if (gameController == nullptr) {
        throw std::runtime_error("No compatible game controller found.");
    }
}

joystick::~joystick() {
    if (gameController != nullptr) {
        SDL_GameControllerClose(gameController);
    }
    SDL_Quit();
}

void joystick::setButtonAction(int button, Actions actions) {
    buttonActions[button] = std::move(actions);
}

void joystick::setAxisAction(int axis, std::function<void(int)> action) {
    axisActions[axis] = std::move(action);
}

void joystick::setPS4AxisMapping(carMove& car) {
    std::cout << "Mapping PS4 controller axes to car actions." << '\n';
    // Left stick X-axis controls the servo (steering)
    setAxisAction(1, [&car](int value) {
        // Normalize SDL axis value (-32768 to 32767) to angle range (-45 to 45 degrees)
        int angle = static_cast<int>(value / 32767.0 * 45);
        car.setServoAngle(angle);
    });

    // Right stick Y-axis controls the motor speed
    setAxisAction(5, [&car](int value) {
        // Normalize SDL axis value (-32768 to 32767) to speed percentage (-100% to 100%)
        int speed = static_cast<int>(value / 32767.0 * 100);
        car.setMotorSpeed(speed);  // Invert to match joystick forward/backward direction
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
                    std::cout << "Button " << button << " pressed." << '\n';
                    buttonActions[button].onPress();
                } else {
                    std::cout << "Button " << button << " released." << '\n';
                    buttonActions[button].onRelease();
                }
            }
        }
    } else if (event.type == SDL_CONTROLLERAXISMOTION) {
        int axis = event.caxis.axis;
        int value = event.caxis.value;

        std::cout << "Axis " << axis << " moved to " << value << "." << '\n';
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
