#include <pigpio.h>

#include <iostream>

#include "gpio_sdl.h"

#define GPIO_BUTTON 17

int main() {
    if (!initGPIO()) {
        return -1;
    }
    gpioSetMode(GPIO_BUTTON, PI_INPUT);
    gpioSetPullUpDown(GPIO_BUTTON, PI_PUD_UP);

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

        int buttonState = readButtonState(GPIO_BUTTON);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (buttonState == 0) {
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
