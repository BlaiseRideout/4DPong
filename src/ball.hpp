#ifndef BALL_H
#define BALL_H

#include "graphics.hpp"

class Ball {
  public:
    glm::vec4 position;
    glm::vec4 velocity;

    Ball();
    void draw(Graphics);
    void update();
};

#endif