#ifndef SPHEREH
#define SPHEREH
#include <stdlib.h>
#include <iostream>
#include "vec3.h"
#include "ray.h"
class sphere
{
  public:
    sphere() {}
    sphere(const vec3 &position, const float radius): pos(position), rad(radius) {}
    vec3 position() const { return pos; }
    float radius() const { return rad; }
    inline bool intersects(const vec3 &point) {return (point - pos).length() <= rad;}
    float hit(const ray &r) const;
    vec3 pos;
    float rad;
};

float sphere::hit(const ray &r) const
{
  vec3 oc = r.origin() - pos;
  float a = dot(r.direction(), r.direction());
  float b = 2 * dot(r.direction(), oc);
  float c = dot(oc, oc) - (rad * rad);
  float det = (b * b) - (4 * a * c);
  if (det > 0)
  {
    return (- b - sqrt(det)) / (2*a);
  } else 
  {
    return -1;
  }
}

inline std::istream& operator>>(std::istream &is, sphere &v)
{
  is >> v.pos >> v.rad;
  return is;
}

inline std::ostream& operator<<(std::ostream &os, const sphere &v)
{
  os << "(" << v.pos << "), " << v.rad;
  return os;
}

#endif