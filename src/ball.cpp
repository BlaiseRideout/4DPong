#include "ball.hpp"
#include "graphics.hpp"

Ball::Ball() {

}

void Ball::draw(Graphics g) {
  g.drawCube((1 + position.w) / 2.0f, glm::vec3(position.x, position.y, position.z), .5f);
}

void Ball::update() {

}