#include <pigpio.h>

#include <iostream>

#include "gpio_sdl.h"

constexpr uint8_t gpio_button = 17;

auto main() -> int {
    if (!initGPIO()) {
        return -1;
    }
    gpioSetMode(gpio_button, PI_INPUT);
    gpioSetPullUpDown(gpio_button, PI_PUD_UP);

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!initSDL(window, renderer)) {
        cleanupGPIO();
        return -1;
    }

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        int button_state = readButtonState(gpio_button);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (button_state == 0) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        SDL_Rect rect = {220, 140, 200, 200};
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    cleanupSDL(window, renderer);
    cleanupGPIO();
    return 0;
}
