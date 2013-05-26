#ifndef PADDLE_H
#define PADDLE_H

#include "graphics.hpp"

class Paddle {
  public:
    glm::vec4 pos;
    int score;

    Paddle(char, char, char, char, char, char, float);

    void update();
    void draw(Graphics);
  private:
    char upKey, downKey, leftKey, rightKey, inKey, outKey;
};

#endif