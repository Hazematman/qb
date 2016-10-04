#ifndef SHADER_HPP
#define SHADER_HPP
#include <string>
#include "gl_core.hpp"

class Shader {
  public:
    void compile(const std::string &vertexFile, const std::string &fragmentFile);
    GLuint getID();
    GLint getUniform(const std::string &name);
  private:
    GLuint programID;
    GLuint vertex, fragment;
    std::string vertexFile, fragmentFile;

    GLuint compileShader(const std::string &file, GLenum type);
    std::string getLog(GLuint object);
    std::string loadFile(const std::string &filename);
};

#endif
