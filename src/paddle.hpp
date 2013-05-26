#ifndef PADDLE_H
#define PADDLE_H

#include "graphics.hpp"

class Paddle {
  public:
    glm::vec4 pos;

    Paddle();

    void update();
    void draw(Graphics);
};

#endif