#version 330

in vec2 uv;
uniform sampler2D textureSampler;
out vec4 frag_color;

void main () {
  float depth = texture(textureSampler, uv).r;
  frag_color = vec4(vec3(depth), 1.0);
}