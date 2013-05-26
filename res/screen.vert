#version 330 core
layout(location = 0) in vec3 vertexPosition;
uniform mat4 MVP;
uniform float in_Color;

out vec4 vertColor;

void main(){
  vec4 v = vec4(vertexPosition, 1);

  gl_Position = MVP * v;

  float shade = 0.5f;

  vertColor = vec4(shade, shade, shade, 1.0);
}
