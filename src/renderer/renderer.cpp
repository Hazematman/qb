#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "renderer/renderer.hpp"
#include "error.hpp"
using namespace std;

struct QuadVert {
  float pos[2];
  float uv[2];
};

static QuadVert quadVerts[] = {
  {{-1.0f, -1.0f},
   {0.0f,  0.0f}},
  {{1.0f, -1.0f},
   {1.0f,  0.0f}},
  {{1.0f,  1.0f},
   {1.0f,  1.0f}},
  {{-1.0f,  1.0f},
   {0.0f,  1.0f}},
};


void Renderer::init(int width, int height) {
  glClearColor(0,0,0,1); 
  glEnable(GL_DEPTH_TEST);

  screen.shader.compile("../data/shaders/screen.vert", "../data/shaders/screen.frag");

  screen.colour = screen.shader.getUniform("colour");
  screen.normal = screen.shader.getUniform("normal");
  screen.pos = screen.shader.getUniform("pos");
  //screen.depth = screen.shader.getUniform("depth");
  screen.lights = screen.shader.getUniform("lights");
  screen.numLights = screen.shader.getUniform("numLights");
  screen.cameraPos = screen.shader.getUniform("cameraPos");

  basic.shader.compile("../data/shaders/basic.vert", "../data/shaders/basic.frag");
  glUseProgram(basic.shader.getID());

  basic.model = basic.shader.getUniform("model");
  basic.modelIT = basic.shader.getUniform("modelIT");
  basic.viewProjection = basic.shader.getUniform("viewProjection");
  basic.colour = basic.shader.getUniform("colour");

  setProj(glm::mat4(1));
  setView(glm::mat4(1));

  glGenVertexArrays(1, &quadVao);
  glBindVertexArray(quadVao);

  glGenBuffers(1, &quadVbo);
  glBindBuffer(GL_ARRAY_BUFFER, quadVbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

  QuadVert *v = NULL;
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
      0,
      2,
      GL_FLOAT,
      false,
      sizeof(QuadVert),
      v->pos);

  glVertexAttribPointer(
      1,
      2,
      GL_FLOAT,
      false,
      sizeof(QuadVert),
      v->uv);

  glBindVertexArray(0);

  this->width = width;
  this->height = height;
  createGbuffer();

  this->numLights = 0;
}

void Renderer::resize(int width, int height) {
  this->width = width;
  this->height = height;

  glBindTexture(GL_TEXTURE_2D, gbuffer[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  glBindTexture(GL_TEXTURE_2D, gbuffer[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  glBindTexture(GL_TEXTURE_2D, gbuffer[2]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, NULL);

  glBindTexture(GL_TEXTURE_2D, gbuffer[3]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

  glViewport(0, 0, width, height);
}

void Renderer::setBasicState() {
  glUseProgram(basic.shader.getID());
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glm::mat4 viewProjection = projection * view;
  glUniformMatrix4fv(basic.viewProjection, 1, false, &viewProjection[0][0]);
}

void Renderer::createGbuffer() {
  glGenFramebuffers(1, &frameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

  glGenTextures(4, gbuffer);

  glBindTexture(GL_TEXTURE_2D, gbuffer[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, gbuffer[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, gbuffer[2]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, gbuffer[3]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  /* Attach depth buffer */
  glGenRenderbuffers(1, &depthbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer); 

  /* Attach Colour buffer */
  for(int i=0; i < 3; i++) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, gbuffer[i], 0);
  }
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gbuffer[3], 0);

  GLenum drawBuffer[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
  glDrawBuffers(sizeof(drawBuffer)/sizeof(GLenum), drawBuffer);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    error("Failed to create gbuffers");
  }
}

void Renderer::setProj(glm::mat4 proj) {
  projection = proj;
}

void Renderer::setProj(int width, int height) {
  projection = glm::perspective<float>(90.0f, width/height, 0.1f, 1000.0f);
}

void Renderer::setView(glm::mat4 view) {
  this->view = view;
}

void Renderer::setView() {
  this->view = camera.view();
}

void Renderer::drawModel(Model &model) {
  /* Set model matrix */
  glm::mat4 modelMat = model.getTransormMat();
  glm::mat3 modelMatInverseTranspose = glm::transpose(glm::inverse(glm::mat3(modelMat)));
  glUniformMatrix4fv(basic.model, 1, false, &modelMat[0][0]);
  glUniformMatrix3fv(basic.modelIT, 1, false, &modelMatInverseTranspose[0][0]);

  /* Set model colour */
  glUniform3fv(basic.colour, 1, &model.colour[0]);

  ModelData *data = model.getData();
  glBindVertexArray(data->vao);
  glDrawElements(GL_TRIANGLES, data->numTriangles*3, GL_UNSIGNED_SHORT, NULL);
}

void Renderer::addLight(glm::vec3 pos) {
  if(numLights < 16) {
    lights[numLights] = pos;
    numLights++;
  }
}

void Renderer::drawFrame() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glUseProgram(screen.shader.getID());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, gbuffer[3]);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, gbuffer[2]);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, gbuffer[1]);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gbuffer[0]);

  glUniform1i(screen.colour, 0);
  glUniform1i(screen.normal, 1);
  glUniform1i(screen.pos, 2);
  glUniform1i(screen.depth, 3);

  glUniform3fv(screen.lights, 16, &lights[0][0]);
  glUniform3fv(screen.cameraPos, 1, &camera.position[0]);
  glUniform1i(screen.numLights, numLights);
  glBindVertexArray(quadVao);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glEnable(GL_DEPTH_TEST);
}
