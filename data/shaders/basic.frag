#version 330

uniform vec3 colour;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec3 outNormal;

smooth in vec3 o_normal;

void main() {
  outColour = vec4(colour, 1.0);
  outNormal = normalize(o_normal);
}
