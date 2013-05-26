#ifndef LEVEL_H
#define LEVEL_H

#include "ball.hpp"
#include "paddle.hpp"

class Level {
  public:
    Ball ball;
    Paddle p1;
    Paddle p2;

    Level();

    void draw(Graphics);
    void update();
};

#endif