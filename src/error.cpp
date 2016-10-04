#include <SDL2/SDL.h>
#include "error.hpp"
using namespace std;

void error(string message) {
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
      "Error", message.c_str(), NULL);
}
