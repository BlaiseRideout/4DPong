#include "level.hpp"

Level::Level() {
  
}

void Level::draw(Graphics g) {
  this->ball.draw(g);
  this->p1.draw(g);
  this->p2.draw(g);
}

void Level::update() {
  this->ball.update();
  this->p1.update();
  this->p2.update();
}