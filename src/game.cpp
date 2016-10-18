#include <iostream>
#include <SDL2/SDL.h>
#include "renderer/model.hpp"
#include "gl_core.hpp"
#include "game.hpp"
#include "timer.hpp"
#include "error.hpp"
using namespace std;

#define BITS_PER_PIXEL 8
#define MOUSE_SENSE 100.0

Game::Game() {
  width = 800;
  height = 600;
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
                                        width, height,
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

  renderer.init(width, height);
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
  Timer timer;
  float dt = 0.0;

  /* Testing code init here */
  float verts[] = {
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
  };

  short indicies[] = {
    4,5,6,
    4,6,7,
    
    5,1,2,
    5,2,6,

    1,2,3,
    1,3,0,
    
    0,3,7,
    0,7,4,

    7,6,2,
    7,2,3,

    4,5,1,
    4,1,0,
  };

  ModelData md(verts, sizeof(verts)/sizeof(float), indicies, sizeof(indicies)/sizeof(short));
  Model quad;
  quad.setModelData(md);
  /* End testing code init */

  const uint8_t *keystate = SDL_GetKeyboardState(NULL);
  while(running) {
    while(SDL_PollEvent(&e)) {
      switch(e.type) {
        case SDL_QUIT:
          running = false;
          break;
        case SDL_KEYDOWN:
          switch(e.key.keysym.sym) {
            case SDLK_ESCAPE:
              running = false;
              break;
          }
          break;
        case SDL_MOUSEMOTION:
          if(SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS) {
            renderer.camera.turn(-e.motion.yrel*dt*MOUSE_SENSE, e.motion.xrel*dt*MOUSE_SENSE);
          }
          break;
        case SDL_WINDOWEVENT:
          switch(e.window.event) {
            case SDL_WINDOWEVENT_FOCUS_GAINED:
              SDL_SetRelativeMouseMode(SDL_TRUE);
              break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
              SDL_SetRelativeMouseMode(SDL_FALSE);
              break;
            case SDL_WINDOWEVENT_RESIZED:
              width = e.window.data1;
              height = e.window.data2;
              renderer.resize(width, height);
              break;
          }
          break;

      }
    }

    if(keystate[SDL_GetScancodeFromKey(SDLK_w)]) {
        renderer.camera.move(10*dt);
    }
    if(keystate[SDL_GetScancodeFromKey(SDLK_s)]) {
      renderer.camera.move(-10*dt);
    }
    if(keystate[SDL_GetScancodeFromKey(SDLK_a)]) {
      renderer.camera.strafe(-10*dt);
    }
    if(keystate[SDL_GetScancodeFromKey(SDLK_d)]) {
      renderer.camera.strafe(10*dt);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer.setProj(width, height);
    renderer.setView();
    renderer.setBasicState();
    renderer.drawModel(quad);

    renderer.drawFrame();

    SDL_GL_SwapWindow(window);

    dt = timer.getElapsedTime();
    timer.reset();
  }
}
