#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <glm/glm.hpp>
#include "renderer/model.hpp"
#include "renderer/shader.hpp"
#include "renderer/camera.hpp"
#include  "gl_core.hpp"

struct ShaderBasic {
  GLuint model;
  GLuint modelIT;
  GLuint viewProjection;
  GLuint colour;
  Shader shader;
};

struct ShaderScreen {
  GLuint colour;
  GLuint normal;
  GLuint depth;
  GLuint pos;
  GLuint lights;
  GLuint numLights;
  GLuint cameraPos;
  Shader shader;
};

class Renderer {
  public:
    Camera camera;

    void init(int width, int height);
    void drawModel(Model &model);
    void drawFrame();

    void resize(int width, int height);
    void setProj(int width, int height);
    void setProj(glm::mat4 proj);
    void setView(glm::mat4 view);
    void setView();
    void setBasicState();
    void addLight(glm::vec3 pos);
  private:
    int width, height;
    glm::vec3 lights[16];
    int numLights;
    glm::mat4 projection;
    glm::mat4 view;
    ShaderBasic basic;
    ShaderScreen screen;

    GLuint quadVbo;
    GLuint quadVao;

    GLuint frameBuffer;

    /* Gbuffer works like this
     * Texture 0 is RGB colour
     * Texture 1 is XYZ normals
     * Texture 2 is XYZ position
     * Texture 3 is depth information
     */
    GLuint gbuffer[4];
    GLuint depthbuffer;

    void createGbuffer();
};

#endif
