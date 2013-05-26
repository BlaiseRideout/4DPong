#include <GL/glfw.h>

#include "paddle.hpp"

Paddle::Paddle(char upKey, char downKey, char leftKey, char rightKey, char inKey, char outKey, float w) {
  this->upKey     = upKey;
  this->downKey   = downKey;
  this->leftKey   = leftKey;
  this->rightKey  = rightKey;
  this->inKey     = inKey;
  this->outKey    = outKey;
  this->pos.w     = w;
  this->pos.x     = w;
  this->pos.y     = w;
  this->pos.z     = w;
}

void Paddle::update() {
  if(glfwGetKey(this->rightKey) != GLFW_RELEASE)
    this->pos.x += 0.01;
  if(glfwGetKey(this->leftKey) != GLFW_RELEASE)
    this->pos.x -= 0.01;
  if(glfwGetKey(this->upKey) != GLFW_RELEASE)
    this->pos.y += 0.01;
  if(glfwGetKey(this->downKey) != GLFW_RELEASE)
    this->pos.y -= 0.01;
  if(glfwGetKey(this->inKey) != GLFW_RELEASE)
    this->pos.z += 0.01;
  if(glfwGetKey(this->outKey) != GLFW_RELEASE)
    this->pos.z -= 0.01;

  if(this->pos.x > 1)
    this->pos.x = 1;
  if(this->pos.x < -1)
    this->pos.x = -1;

  if(this->pos.y > 1)
    this->pos.y = 1;
  if(this->pos.y < -1)
    this->pos.y = -1;
  
  if(this->pos.z > 1)
    this->pos.z = 1;
  if(this->pos.z < -1)
    this->pos.z = -1;
}

void Paddle::draw(Graphics g) {
  g.drawCube((1 + pos.w) / 2.0f, 1.0f, glm::vec3(pos.x, pos.y, pos.z), .2f);
}