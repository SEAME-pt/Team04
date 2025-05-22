#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>

#include <array>
#include <functional>
#include <iostream>
#include <map>
#include <utility>

#include "platform/carMove/carMove.hpp"
#include "platform/remoteMove/remoteMove.hpp"

struct Actions {
    std::function<void()> onPress;
    std::function<void()> onRelease;
};

class joystick {
   public:
    joystick(carMove& car);
    ~joystick();
    void setButtonAction(int button, Actions actions);
    void setAxisAction(int axis, std::function<void(int)> action);
    void setAxisMapping(carMove& car);
    void listen();
    void printButtonStates();

   private:
    SDL_GameController* gameController;
    std::map<int, Actions> buttonActions;
    std::map<int, std::function<void(int)>> axisActions;
    std::array<bool, SDL_CONTROLLER_BUTTON_MAX> buttonStates{};
    void processEvent(const SDL_Event& event);
    RemoteMove* remote;

    int shutdownButton = 7;
    std::chrono::steady_clock::time_point shutdownButtonPressTime;
    bool isShutdownButtonHeld = false;
    const std::chrono::seconds shutdownHoldDuration{3};

    carMove* car_ref;
};
