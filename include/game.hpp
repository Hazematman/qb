#ifndef GAME_HPP
#define GAME_HPP
#include <SDL2/SDL.h>
#include "renderer/renderer.hpp"

class Game {
  public:
    Game();
    void run();
  private:
    SDL_Window *window;
    SDL_GLContext context;
    int width, height;
    Renderer renderer;
    void checkSDLError();
};

#endif
