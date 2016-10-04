#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <glm/glm.hpp>
#include "model.hpp"

class Renderer {
  public:
    void init();
    void drawModel(Model &model);
  private:
    glm::mat4 projection;
    glm::mat4 view;
};

#endif
