#include "level.hpp"

Level::Level() : 
p1(',', 'O', 'A', 'E', '\'', '.', -1.0f),
p2('C', 'T', 'H', 'N', 'G', 'R', 1.0f) {

}

void Level::draw(Graphics g) {
  this->ball.draw(g);
  this->p1.draw(g);
  this->p2.draw(g);
}

void Level::update() {
  this->p1.update();
  this->p2.update();
  this->ball.update(p1, p2);
}