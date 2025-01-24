#ifndef GPIO_SDL_H
#define GPIO_SDL_H

#include <SDL2/SDL.h>

bool initGPIO();
int readButtonState(int gpioPin);
bool initSDL(SDL_Window*& window, SDL_Renderer*& renderer);
void cleanupSDL(SDL_Window* window, SDL_Renderer* renderer);
void cleanupGPIO();

#endif  // GPIO_SDL_H
