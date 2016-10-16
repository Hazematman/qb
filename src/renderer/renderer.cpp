#include <glm/gtc/matrix_transform.hpp>
#include "renderer/renderer.hpp"

void Renderer::init() {
  glClearColor(0,0,0,1); 
  glEnable(GL_DEPTH_TEST);

  basic.shader.compile("../data/shaders/basic.vert", "../data/shaders/basic.frag");
  glUseProgram(basic.shader.getID());

  basic.model = basic.shader.getUniform("model");
  basic.viewProjection = basic.shader.getUniform("viewProjection");
  basic.colour = basic.shader.getUniform("colour");

  setProj(glm::mat4(1));
  setView(glm::mat4(1));

}

void Renderer::setBasicState() {
  glUseProgram(basic.shader.getID());
  glm::mat4 viewProjection = projection * view;
  glUniformMatrix4fv(basic.viewProjection, 1, false, &viewProjection[0][0]);
}

void Renderer::setProj(glm::mat4 proj) {
  projection = proj;
}

void Renderer::setView(glm::mat4 view) {
  this->view = view;
}

void Renderer::drawModel(Model &model) {
  /* Set model matrix */
  glm::mat4 modelMat = glm::translate(glm::mat4(1), model.pos);
  glUniformMatrix4fv(basic.model, 1, false, &modelMat[0][0]);

  /* Set model colour */
  glUniform3fv(basic.colour, 1, &model.colour[0]);

  ModelData *data = model.getData();
  glBindVertexArray(data->vao);
  glDrawElements(GL_TRIANGLES, data->numTriangles*3, GL_UNSIGNED_SHORT, NULL);
}
