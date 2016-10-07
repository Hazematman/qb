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
    GLuint vbo, ibo;
    GLuint vao;
    int numTriangles;
    std::vector<short> indicies;

    ModelData(Vertex *vertices, size_t v_size, short *indicies, size_t i_size);
    ModelData(float *vertices, size_t f_size, short *indicies, size_t i_size);
  private:
    std::vector<Vertex> vertices;

    void createVao();
    void createIbo(short *indicies, size_t size);
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
