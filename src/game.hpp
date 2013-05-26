#ifndef GAME_H
#define GAME_H

#include "level.hpp"
#include "graphics.hpp"

class Game {
  public:
    Game();

    void update();
    void draw();
    void init();
    void run();
    void cleanup();
    
  private:
    bool      running;
    Graphics  graphics;
    Level     level;
};

#endif