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
  this->velocity.w = rand() % 2 * 2 - 1;

  glm::normalize(this->velocity);
  this->velocity *= 0.0005f;
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

  if(this->position.w < p1.pos.w - 0.2f) {
    p2.score++;
    reset();
  }
  if(this->position.w > p2.pos.w + 0.2f) {
    p1.score++;
    reset();
  }

  if((this->position.w > p1.pos.w - 0.05f && this->position.w < p1.pos.w + 0.05f &&
      this->position.x > p1.pos.x - 0.2f && this->position.x < p1.pos.x + 0.2f &&
      this->position.y > p1.pos.y - 0.2f && this->position.y < p1.pos.y + 0.2f &&
      this->position.z > p1.pos.z - 0.2f && this->position.z < p1.pos.z + 0.2f) ||
     (this->position.w > p2.pos.w - 0.05f && this->position.w < p2.pos.w + 0.05f &&
      this->position.x > p2.pos.x - 0.2f && this->position.x < p2.pos.x + 0.2f &&
      this->position.y > p2.pos.y - 0.2f && this->position.y < p2.pos.y + 0.2f &&
      this->position.z > p2.pos.z - 0.2f && this->position.z < p2.pos.z + 0.2f)) {
    this->velocity.w *= -1;
  }

  this->position += this->velocity;
}