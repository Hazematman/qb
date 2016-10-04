#ifndef MODEL_HPP
#define MODEL_HPP
#include <glm/glm.hpp>
#include <gl_core.hpp>
#include <vector>

struct Vertex {
  glm::vec3 position;
};

class ModelData  {
  private:
    std::vector<Vertex> vertices;
    GLint vbo;
};

class Model {
  private:
    ModelData *data;
};

#endif
