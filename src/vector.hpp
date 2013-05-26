#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

class Vec4 {
  public:
    float x, y, z, w;

    Vec4(float x, float y, float z, float w);
    Vec4();

    float length();
    void  normalize();
};

class Vec3 {
  public:
    float x, y, z;

  Vec3(float x, float y, float z);
  Vec3();

  float length();
  void  normalize();
};

struct Vec2 {
  public:
    float x, y;

    Vec2(float x, float y);
    Vec2();

    float length();
    void  normalize();

};
#endif