#include <GL/glfw.h>
#include <glm/gtc/matrix_transform.hpp>

#include "paddle.hpp"
#include "graphics.hpp"

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
  this->score     = 0;
}

void Paddle::update() {
  if(glfwGetKey(this->rightKey) != GLFW_RELEASE) {
    glm::vec4 cameraPosition = glm::rotate(glm::mat4(1.0f), Graphics::cameraRotation, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(Graphics::cameraPosition, 0.0f);
    glm::vec3 direction = glm::vec3(this->pos) - glm::vec3(cameraPosition);
    direction.y = 0;
    glm::normalize(direction);
    glm::vec4 direction4 = glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(0, 1, 0)) * glm::vec4(direction, 1.0f);
    glm::normalize(direction4);
    this->pos.z -= 0.02 * direction4.z;
    this->pos.x -= 0.02 * direction4.x;
  }
  if(glfwGetKey(this->leftKey) != GLFW_RELEASE) {
    glm::vec4 cameraPosition = glm::rotate(glm::mat4(1.0f), Graphics::cameraRotation, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(Graphics::cameraPosition, 0.0f);
    glm::vec3 direction = glm::vec3(this->pos) - glm::vec3(cameraPosition);
    direction.y = 0;
    glm::normalize(direction);
    glm::vec4 direction4 = glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(0, 1, 0)) * glm::vec4(direction, 1.0f);
    glm::normalize(direction4);
    this->pos.x += 0.02 * direction4.x;
    this->pos.z += 0.02 * direction4.z;
  }

  if(glfwGetKey(this->inKey) != GLFW_RELEASE) {
    glm::vec4 cameraPosition = glm::rotate(glm::mat4(1.0f), Graphics::cameraRotation, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(Graphics::cameraPosition, 0.0f);
    glm::vec3 direction = glm::vec3(this->pos) - glm::vec3(cameraPosition);
    direction.y = 0;
    glm::normalize(direction);
    this->pos.x -= 0.02 * direction.x;
    this->pos.z -= 0.02 * direction.z;
  }
  if(glfwGetKey(this->outKey) != GLFW_RELEASE) {
    glm::vec4 cameraPosition = glm::rotate(glm::mat4(1.0f), Graphics::cameraRotation, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(Graphics::cameraPosition, 0.0f);
    glm::vec3 direction = glm::vec3(this->pos) - glm::vec3(cameraPosition);
    direction.y = 0;
    glm::normalize(direction);
    this->pos.z += 0.02 * direction.z;
    this->pos.x += 0.02 * direction.x;
  }

  if(glfwGetKey(this->upKey) != GLFW_RELEASE)
    this->pos.y += 0.01;
  if(glfwGetKey(this->downKey) != GLFW_RELEASE)
    this->pos.y -= 0.01;

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
