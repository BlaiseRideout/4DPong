#include "game.hpp"
#include <GL/glfw.h>
#include <time.h>

Game::Game() {
  this->running = true;
}

void Game::update() {
  running = running && glfwGetKey(GLFW_KEY_ESC) == GLFW_RELEASE;
  
  this->level.update();
}

void Game::draw() {
  graphics.clearScreen();
  graphics.prepareContext();
  this->level.draw(this->graphics);
  glfwSwapBuffers();
}

void Game::init() {
  this->graphics.init(1280, 600, false);

  srand(time(NULL));
}

void Game::run() {
  while(this->running && glfwGetWindowParam(GLFW_OPENED)) {
    update();
    draw();
  }
}

void Game::cleanup() {
  glfwTerminate();
}
