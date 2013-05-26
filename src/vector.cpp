#include "vector.hpp"

Vec4::Vec4(float x, float y, float z, float w)  {
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
}

Vec4::Vec4() {
  Vec4(0, 0, 0, 0);
}

float Vec4::length() {
  return sqrt(x * x + y * y + z * z + w * w);
}

void Vec4::normalize()  {
  float length = this->length();
  x /= length;
  y /= length;
  z /= length;
  w /= length;
}

float Vec3::length() {
  return sqrt(x * x + y * y + z * z);
}

void Vec3::normalize() {
  float length = this->length();
  x /= length;
  y /= length;
  z /= length;
}

Vec3::Vec3(float x, float y, float z)  {
  this->x = x;
  this->y = y;
  this->z = z;
}

Vec3::Vec3() {
  Vec3(0, 0, 0);
}

float Vec2::length() {
  return sqrt(x * x + y * y);
}

void Vec2::normalize() {
  float length = this->length();
  x /= length;
  y /= length;
}

Vec2::Vec2(float x, float y)  {
  this->x = x;
  this->y = y;
}

Vec2::Vec2() {
  Vec2(0, 0);
}