#include "gpio_sdl.h"

#include <pigpio.h>

#include <iostream>

auto initGPIO() -> bool {
    if (gpioInitialise() < 0) {
        std::cerr << "Failed to initialize pigpio." << '\n';
        return false;
    }
    return true;
}

auto readButtonState(int gpioPin) -> int { return gpioRead(static_cast<unsigned int>(gpioPin)); }

auto initSDL(SDL_Window*& window, SDL_Renderer*& renderer) -> bool {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << '\n';
        return false;
    }
    window = SDL_CreateWindow("GPIO + SDL2 Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              640, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << '\n';
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create SDL renderer: " << SDL_GetError() << '\n';
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
