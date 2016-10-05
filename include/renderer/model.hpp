#ifndef MODEL_HPP
#define MODEL_HPP
#include <glm/glm.hpp>
#include <gl_core.hpp>
#include <vector>

struct Vertex {
  glm::vec3 position;
};

class ModelData  {
  public:
    GLuint vbo;
    GLuint vao;
    int numTriangles;

    ModelData(Vertex *vertices, size_t size);
    ModelData(float *vertices, size_t size);
  private:
    std::vector<Vertex> vertices;

    void createVao();
};

class Model {
  public:
    glm::vec3 position;
    glm::vec3 colour;

    Model();
    void setModelData(ModelData &data);
    ModelData *getData();
  private:
    ModelData *data;
};

#endif
