#version 330

uniform sampler2D colour;
uniform sampler2D normal;
uniform sampler2D pos;
//uniform sampler2D depth;

uniform vec3 lights[16];
uniform int numLights;
uniform vec3 cameraPos;

smooth in vec2 o_uv;
out vec4 outColour; 

void main() {
  vec4 colourP = texture(colour, o_uv);
  vec4 normalP = texture(normal, o_uv);
  vec4 posP = 2*texture(pos, o_uv) - vec4(1,1,1,1);
  //vec4 depthP = texture(depth, o_uv);

  vec3 diffuse = vec3(0,0,0);
  for(int i=0; i < numLights; i++) {
    float distance = length(lights[i] - posP.xyz);
    vec3 direction = normalize(lights[i] - posP.xyz);
    diffuse = vec3(1,1,1)*colourP.xyz*max(0, dot(normalP.xyz, direction));
  }

  outColour = vec4(diffuse, 1);
  outColour = normalP;
}
