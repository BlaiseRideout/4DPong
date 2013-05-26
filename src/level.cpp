#include <string>
#include <math.h>
#include <GL/glut.h>

#include "level.hpp"

static std::string itos(int);
static int apow(const int &x, int y);
static std::string digit2str(short digit);


Level::Level() : 
p1(',', 'O', 'A', 'E', '\'', '.', -1.0f),
p2('C', 'T', 'H', 'N', 'G', 'R', 1.0f) {

}

void Level::draw(Graphics g) {
  this->ball.draw(g);
  this->p1.draw(g);
  this->p2.draw(g);

//  std::string p1score = itos(this->p1.score);
//  for(unsigned int i = 0; i < p1score.length(); ++i)
//    glutStrokeCharacter(GLUT_STROKE_ROMAN, p1score[i]);
}

void Level::update() {
  this->p1.update();
  this->p2.update();
  this->ball.update(p1, p2);
}

static std::string itos(int a) {
  /* Convert an integer to a string */
  int b = 0, c = 1, n = 0;
  std::string result;
  while(apow(10,n) < a) n++;
  if(n != 0) n--;
  b = apow(10,n);
  while((b*c) < a) c++;
  if(n != 0) c--;
  a -= (b*c);
  result += digit2str(c);
  if(n != 0) result += itos(a);
  return result;
}

static int apow(const int &x, int y){
  /* Raise x to the power of y */
  int result = 1;
  if(y < 0)
  return apow(1/x, -y);
  for(int i = y; i > 0; i--)
  result *= x;
  return result; 
}

static std::string digit2str(short digit){
  /* Convert a digit to a string */
  std::string tmp;
  if(digit >= 0 && digit <= 9)
  {
    tmp = digit + '0';
    return tmp;
  }
  return "";
}