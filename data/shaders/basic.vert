#version 330
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 viewProjection;
uniform mat4 model;
uniform mat3 modelIT;

smooth out vec3 o_normal;
smooth out vec3 o_pos;

void main() {
  vec4 modelPos = model * vec4(position, 1.0);
  gl_Position = viewProjection * modelPos;
  o_normal = modelIT*normal;
  o_pos = vec3(modelPos); 
}
