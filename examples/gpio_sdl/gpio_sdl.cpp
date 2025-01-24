#include "gpio_sdl.h"

#include <pigpio.h>

#include <iostream>

bool initGPIO() {
    if (gpioInitialise() < 0) {
        std::cerr << "Failed to initialize pigpio." << std::endl;
        return false;
    }
    return true;
}

int readButtonState(int gpioPin) { return gpioRead(static_cast<unsigned int>(gpioPin)); }

bool initSDL(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("GPIO + SDL2 Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              640, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create SDL renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return false;
    }
    return true;
}

void cleanupSDL(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void cleanupGPIO() { gpioTerminate(); }
