#include "renderer/model.hpp"
using namespace std;

ModelData::ModelData(Vertex *vertices, size_t v_size, short *indicies, size_t i_size) {
  for(size_t i=0; i < v_size; i++) {
    this->vertices.push_back(vertices[i]);
  }
  for(size_t i=0; i < i_size; i++) {
    this->indicies.push_back(indicies[i]);
  }
  numTriangles = i_size / 3;

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, v_size*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  createIbo(indicies, i_size);

  createVao();
}

ModelData::ModelData(float *vertices, size_t f_size, short *indicies, size_t i_size) {
  for(size_t i=0; i < f_size; i+=3) {
    Vertex v;
    v.position = glm::vec3(vertices[i], vertices[i+1], vertices[i+2]);
    this->vertices.push_back(v);
  }

  for(size_t i=0; i < i_size; i++) {
    this->indicies.push_back(indicies[i]);
  }
  numTriangles = i_size/3;

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, f_size*sizeof(float), vertices, GL_STATIC_DRAW);

  createIbo(indicies, i_size);

  createVao();
}

void ModelData::createIbo(short *indicies, size_t size) {
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size*sizeof(short), indicies, GL_STATIC_DRAW);
}

void ModelData::createVao() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
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
