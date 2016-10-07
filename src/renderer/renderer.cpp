#include <glm/gtc/matrix_transform.hpp>
#include "renderer/renderer.hpp"
#include "error.hpp"

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

  createGbuffer();
}

void Renderer::setBasicState() {
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
  glUseProgram(basic.shader.getID());
  glm::mat4 viewProjection = projection * view;
  glUniformMatrix4fv(basic.viewProjection, 1, false, &viewProjection[0][0]);
}

void Renderer::createGbuffer() {
  glGenFramebuffers(1, &frameBuffer);
  glBindBuffer(GL_FRAMEBUFFER, frameBuffer);

  glGenTextures(2, gbuffer);

  /* TODO Handle screen resizing */
  glBindTexture(GL_TEXTURE_2D, gbuffer[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, gbuffer[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  /* Attach depth buffer */
  glGenRenderbuffers(1, &depthbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer); 

  /* Attach Colour buffer */
  for(int i=0; i < 2; i++) {
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, gbuffer[i], 0);
  }

  GLenum drawBuffer[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
  glDrawBuffers(2, drawBuffer);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    error("Failed to create gbuffers");
  }
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
