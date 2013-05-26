#version 330 core

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 vertexUV;

uniform vec2 pos;
uniform mat4 mat;

out vec2 UV;
 
void main(void) {
  gl_Position = mat * vec4(pos.x, pos.y, 0, 1);

  UV = vertexUV;
}