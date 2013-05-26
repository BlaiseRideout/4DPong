#include "game.hpp"
#include <GL/glfw.h>

Game::Game() {
  this->running = true;
}

void Game::update() {
  running = running && glfwGetKey('Q') == GLFW_RELEASE;

  if(glfwGetKey('A') != GLFW_RELEASE)
    this->graphics.cameraPosition.x += 0.05;
  if(glfwGetKey('E') != GLFW_RELEASE)
    this->graphics.cameraPosition.x -= 0.05;
  if(glfwGetKey(',') != GLFW_RELEASE)
    this->graphics.cameraPosition.y += 0.05;
  if(glfwGetKey('O') != GLFW_RELEASE)
    this->graphics.cameraPosition.y -= 0.05;
  
  this->level.update();
}

void Game::draw() {
  graphics.clearScreen();
  graphics.prepareContext();
  this->level.draw(this->graphics);
  glfwSwapBuffers();
}

void Game::init() {
  this->graphics.init(1024, 768, false);
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