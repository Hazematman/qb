#ifndef GAME_HPP
#define GAME_HPP
#include <SDL2/SDL.h>

class Game {
  public:
    Game();
    void run();
  private:
    SDL_Window *window;
    SDL_GLContext context;
    void checkSDLError();
};

#endif
