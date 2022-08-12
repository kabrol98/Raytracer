#ifndef SPHEREH
#define SPHEREH
#include <stdlib.h>
#include <iostream>
#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "materials.h"

// class material;

/// Defined by position vector and radius.
class sphere : public hitable
{
  public:
    sphere() {}
    sphere(const vec3 &position, const float radius, material *m): center(position), rad(radius), mat(m) {
      // cout << "Saved allocated material pointer " << m << "\n";
    }
    virtual ~sphere() {delete mat;} /// Free material pointer.
    float radius() const { return rad; }
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
    vec3 center;
    float rad;
    material *mat;
};

/// @brief Sphere hit method
///
/// Overloaded hitable hit method. Uses sphere intersection equation:
/// For some t, the light ray P(t) = A + tB. L intersects a sphere with center C and radius r 
/// iff ||(A + tB) - C|| = r, or dot(A-C + tB, A-C + tB) = r^2.
/// this becomes (A-C)^2 - r^2 + 2(A-C)(tB) + (tB)^2 = 0. If t has a real value then
/// we have an intersection!
bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const
{
  vec3 oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = 2 * dot(r.direction(), oc);
  float c = dot(oc, oc) - (rad * rad);
  float det = (b * b) - (4 * a * c);
  if (det > 0)
  {
    /// Ensure t is within given bounds. 
    float t = (- b - sqrt(det)) / (2*a);
    if (t_min < t && t < t_max)
    {
      /// Sphere was hit! 
      rec.t = t;
      /// Hit point P(t) can be computed from the ray
      rec.p = r.point_at_parameter(rec.t);
      /// Compute normal = unit vector of P - C
      rec.normal =  (rec.p - center) / radius();
      /// Assign material to hit record.
      rec.mat = mat;
      return true;
    } else {
      return false;
    }
  } else 
  {
    return false;
  }
}

inline std::istream& operator>>(std::istream &is, sphere &v)
{
  is >> v.center >> v.rad;
  return is;
}

inline std::ostream& operator<<(std::ostream &os, const sphere &v)
{
  os << "(" << v.center << "), " << v.rad;
  return os;
}

#endif