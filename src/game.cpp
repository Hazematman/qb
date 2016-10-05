#include <iostream>
#include <SDL2/SDL.h>
#include "renderer/model.hpp"
#include "gl_core.hpp"
#include "game.hpp"
#include "error.hpp"
using namespace std;

#define BITS_PER_PIXEL 8

Game::Game() {
  if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    error("Failed to initalize SDL");
  }

  checkSDLError();

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, BITS_PER_PIXEL);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, BITS_PER_PIXEL);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, BITS_PER_PIXEL);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, BITS_PER_PIXEL);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); 

  window = SDL_CreateWindow("Quantum Boomerang",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        800, 600,
                                        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  checkSDLError();

  context = SDL_GL_CreateContext(window);
  checkSDLError();

  if(ogl_LoadFunctions() == ogl_LOAD_FAILED) {
    error("Failed to initalize OpenGL");
  }

  if(!ogl_IsVersionGEQ(3,3)) {
    error("OpenGL 3.3 not supported");
  }

  renderer.init();
}

void Game::checkSDLError() {
  const char *errorMessage = SDL_GetError();

  if(*errorMessage != '\0') {
    error(string(errorMessage));
  }
}

void Game::run() {
  SDL_Event e;
  bool running = true;

  /* Testing code init here */
  float verts[] = {
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
  };

  ModelData md(verts, sizeof(verts)/sizeof(float));
  Model quad;
  quad.setModelData(md);
  /* End testing code init */

  while(running) {
    while(SDL_PollEvent(&e)) {
      switch(e.type) {
        case SDL_QUIT:
          running = false;
      }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer.setBasicState();
    renderer.drawModel(quad);

    SDL_GL_SwapWindow(window);
  }
}
