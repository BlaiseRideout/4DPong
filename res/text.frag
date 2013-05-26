#version 330
 
in vec2 UV;

uniform sampler2D tex;

out vec4 color;

void main(void) {
  //color = vec4(1, 1, 1, (texture(tex, UV).r + texture(tex, UV).g + texture(tex, UV).b) / 3);
  //color = vec4(texture(tex, UV).rgb, 1);
  color = vec4(1.0, 1.0, 1.0, 1.0);
}