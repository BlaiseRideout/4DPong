#include "game.hpp"
#include <GL/glfw.h>
#include <time.h>

#include <iostream>

Game::Game() {
  this->running = true;
}

void Game::update() {
  running = running && glfwGetKey('Q') == GLFW_RELEASE;
  
  if(this->graphics.cameraRotation < 0)
    this->graphics.cameraRotation -= 0.05;
  else
    this->graphics.cameraRotation += 0.05;

  if(this->graphics.cameraRotation >= 360)
    this->graphics.cameraRotation -= 360;

  if(this->graphics.cameraRotation >= 45 && this->graphics.cameraRotation < 90)
    this->graphics.cameraRotation -= 360;
  if(this->graphics.cameraRotation <= -360 - 45)
    this->graphics.cameraRotation += 360 * 2;

  this->level.update();
}

void Game::draw() {
  graphics.clearScreen();
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