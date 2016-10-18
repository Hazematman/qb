#include "timer.hpp"

#define TICKS_PER_SECOND 1000.0f

Timer::Timer() {
    reset();
}

void Timer::reset() {
    ticks = SDL_GetTicks();
}

float Timer::getElapsedTime() {
    return (SDL_GetTicks() - ticks)/TICKS_PER_SECOND;
}

uint32_t Timer::getElapsedTicks() {
    return (SDL_GetTicks() - ticks);
}
