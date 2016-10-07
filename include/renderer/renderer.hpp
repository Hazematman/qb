#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <glm/glm.hpp>
#include "renderer/model.hpp"
#include "renderer/shader.hpp"
#include  "gl_core.hpp"

struct ShaderBasic {
  GLuint model;
  GLuint viewProjection;
  GLuint colour;
  Shader shader;
};

class Renderer {
  public:
    void init();
    void drawModel(Model &model);

    void setProj(glm::mat4 proj);
    void setView(glm::mat4 view);
    void setBasicState();
  private:
    glm::mat4 projection;
    glm::mat4 view;
    ShaderBasic basic;

    GLuint frameBuffer;

    /* Gbuffer works like this
     * Texture 0 is RGB colour
     * Texture 1 is XYZ normals
     */
    GLuint gbuffer[3];
    GLuint depthbuffer;

    void createGbuffer();
};

#endif
