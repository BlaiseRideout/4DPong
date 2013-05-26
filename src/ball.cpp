#include "ball.hpp"
#include "graphics.hpp"

Ball::Ball() {
  this->reset();
}

void Ball::reset() {
  this->position *= 0;

  this->velocity.x = rand() % 10 - 5;
  this->velocity.y = rand() % 10 - 5;
  this->velocity.z = rand() % 10 - 5;
  this->velocity.w = rand() % 10 - 5;

  glm::normalize(this->velocity);
  this->velocity *= 0.005f;
}

void Ball::draw(Graphics g) {
  g.drawCube((1 + position.w) / 2.0f, 1.0f, glm::vec3(position.x, position.y, position.z), .03f);
}

void Ball::update(Paddle p1, Paddle p2) {
  this->position += this->velocity;

  if(this->position.x > 1 || this->position.x < -1)
    this->velocity.x *= -1;

  if(this->position.y > 1 || this->position.y < -1)
    this->velocity.y *= -1;
  
  if(this->position.z > 1 || this->position.z < -1)
    this->velocity.z *= -1;


}