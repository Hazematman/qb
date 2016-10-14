#version 330

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;

smooth out vec2 o_uv;

void main() {
  gl_Position = vec4(position, 0.0, 1.0);
  o_uv = uv;
}
