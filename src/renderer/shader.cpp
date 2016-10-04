#include <iostream>
#include <fstream>
#include "renderer/shader.hpp"
#include "error.hpp"
using namespace std;

GLuint Shader::getID() {
  return programID;
}

GLint Shader::getUniform(const string &name) {
  GLint location = glGetUniformLocation(programID, name.c_str());
  if(location == -1) {
    error(vertexFile+" "+fragmentFile+" Can't find uniform: "+name);
  }

  return location;
}

void Shader::compile(const string &vertex, const string &fragment) {
  vertexFile = vertex;
  fragmentFile = fragment;
  this->vertex = compileShader(vertex, GL_VERTEX_SHADER);
  this->fragment = compileShader(fragment, GL_FRAGMENT_SHADER);

  programID = glCreateProgram();
  glAttachShader(programID, this->vertex);
  glAttachShader(programID, this->fragment);
  glLinkProgram(programID);

  GLint linkOk = false;
  glGetProgramiv(programID, GL_LINK_STATUS, &linkOk);
  if(linkOk == false) {
    error(getLog(programID));
  }
}

GLuint Shader::compileShader(const string &file, GLenum type) {
  string data = loadFile(file);
  const char *source = data.c_str();
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  GLint compileOk = false;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compileOk);
  if(compileOk == false) {
    error(file +":\n"+getLog(shader));
  }

  return shader;
}

string Shader::loadFile(const string &filename) {
  ifstream file(filename);
  if(file.is_open() == false) {
    error("Cannot open shader file " + filename);
  }

  /* Weird c++ thing to load entire file into a string */
  return string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
}

string Shader::getLog(GLuint object) {
  GLint logLength = 0;
  if(glIsShader(object)) {
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &logLength);
  } else {
    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &logLength);
  }

  char *log = new char[logLength];
  if(glIsShader(object)) {
    glGetShaderInfoLog(object, logLength, NULL, log);
  } else {
    glGetProgramInfoLog(object, logLength, NULL, log);
  }

  string out = log;
  delete[] log;
  return out;
}
