#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 viewProjection;
uniform mat4 model;
uniform mat3 modelIT;

smooth out vec3 o_normal;

void main() {
  gl_Position = viewProjection * model * vec4(position, 1.0);
  o_normal = modelIT*normal;
}
