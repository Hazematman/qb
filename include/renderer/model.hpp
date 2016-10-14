#ifndef MODEL_HPP
#define MODEL_HPP
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <gl_core.hpp>
#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
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
    void createNormals();
};

class Model {
public:
	glm::vec3 pos;
	glm::quat rot;
	glm::vec3 scale; 

	glm::vec3 colour;

	Model();
	Model(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
	      glm::vec3 colour, ModelData *data);
	void setModelData(ModelData &data);
	ModelData *getData();
	glm::mat4 getTransormMat();


private:
	ModelData *data;
};

#endif
