#include <string>
#include <math.h>
#include <GL/glut.h>
#include <iostream>

#include "level.hpp"

Level::Level() : 
p1(',', 'O', 'A', 'E', '\'', '.', -1.0f),
p2('C', 'T', 'H', 'N', 'G', 'R', 1.0f) {

}

void Level::draw(Graphics g) {
  this->ball.draw(g);
  this->p1.draw(g);
  this->p2.draw(g);

  std::string p1score = std::to_string(p1.score);
  //std::cout << p1score << std::endl;
  g.drawText(glm::vec2(0, 0), p1score);
}

void Level::update() {
  this->p1.update();
  this->p2.update();
  this->ball.update(p1, p2);
}