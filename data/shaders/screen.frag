#version 330

uniform sampler2D colour;
uniform sampler2D normal;
uniform sampler2D depth;

smooth in vec2 o_uv;
out vec4 outColour; 

void main() {
  vec4 colourP = texture(colour, o_uv);
  vec4 normalP = texture(normal, o_uv);
  vec4 depthP = texture(depth, o_uv);
  outColour = /*0.0001*/colourP + 0.0001*normalP + 0.0001*depthP;
}
