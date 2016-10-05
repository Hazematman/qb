#include "renderer/model.hpp"
using namespace std;

ModelData::ModelData(Vertex *vertices, size_t size) {
  for(size_t i=0; i < size; i++) {
    this->vertices.push_back(vertices[i]);
  }
  numTriangles = size;

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  createVao();
}

ModelData::ModelData(float *vertices, size_t size) {
  for(size_t i=0; i < size; i+=3) {
    Vertex v;
    v.position = glm::vec3(vertices[i], vertices[i+1], vertices[i+2]);
    this->vertices.push_back(v);
  }

  numTriangles = size/3;

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), vertices, GL_STATIC_DRAW);

  createVao();
}

void ModelData::createVao() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  Vertex *p = NULL;
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
      0,
      3,
      GL_FLOAT,
      false,
      sizeof(Vertex),
      &p->position);
}

ModelData *Model::getData() {
  return data;
}

Model::Model() {
  position = glm::vec3(0,0,0);
  colour = glm::vec3(1,1,1);
}

void Model::setModelData(ModelData &data) {
  this->data = &data;
}
