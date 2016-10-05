#version 330

uniform vec3 colour;

out vec4 outColour;

void main() {
  outColour = vec4(colour, 1.0);
}
