#version 330

uniform sampler2D colour;
uniform sampler2D normal;

smooth in vec2 o_uv;
out vec4 outColour; 

void main() {
  vec4 colourP = texture(colour, o_uv);
  vec4 normalP = texture(normal, o_uv);
  outColour = colourP + normalP;
}
