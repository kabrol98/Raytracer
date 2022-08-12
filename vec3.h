#ifndef VEC3H
#define VEC3H
#include <stdlib.h>
#include <math.h>

class vec3 {
public:
  vec3() {}
  vec3(float x, float y, float z) {e[0] = x; e[1] = y; e[2] = z; }
  inline float x() const {return e[0];}
  inline float y() const {return e[1];}
  inline float z() const {return e[2];}

  inline float r() const {return e[0];}
  inline float g() const {return e[1];}
  inline float b() const {return e[2];}

  inline const vec3& operator+() const {return *this;}
  inline vec3 operator-() const {return vec3(-e[0],-e[1],-e[2]);}
  inline float operator[](int i) const {return e[i];}
  inline float& operator[](int i) { return e[i]; };

  inline vec3& operator+=(const vec3 &v0);
  inline vec3& operator-=(const vec3 &v0);
  inline vec3& operator*=(const vec3 &v0);
  inline vec3& operator/=(const vec3 &v0);
  inline vec3& operator*=(const float t);
  inline vec3& operator/=(const float t);

  inline vec3 sqrt3() const { return vec3(sqrt(e[0]), sqrt(e[1]), sqrt(e[2])); } 
  inline float squared_length() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }
  inline float length() const {
    return sqrt(this->squared_length());
  }
  inline void make_unit_vector();

  float e[3];
};


inline std::istream& operator>>(std::istream &is, vec3 &v)
{
  is >> v.e[0] >> v.e[1] >> v.e[2];
  return is;
}

inline std::ostream& operator<<(std::ostream &os, const vec3 &v)
{
  os << "[" << v.e[0] << " " << v.e[1] << " " << v.e[2] << "]";
  return os;
}

inline void vec3::make_unit_vector() {
  float k = 1.0 / this->length();
  e[0] *= k; e[1] *= k; e[2] *= k;
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2)
{
  return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2)
{
  return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2)
{
  return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2)
{
  return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const float t)
{
  return vec3(v1.e[0] * t, v1.e[1] * t, v1.e[2] * t);
}

inline vec3 operator*(const float t, const vec3 &v1)
{
  return vec3(v1.e[0] * t, v1.e[1] * t, v1.e[2] * t);
}

inline vec3 operator/(const vec3 &v1, const float t)
{
  return vec3(v1.e[0] / t, v1.e[1] / t, v1.e[2] / t);
}

inline vec3 operator/(const float t, const vec3 &v1)
{
  return vec3(v1.e[0] / t, v1.e[1] / t, v1.e[2] / t);
}

inline float dot(const vec3 &v1, const vec3 &v2) {
  return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2]; 
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
  return vec3(
    (v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
    (v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0]),
    (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0])
  );
}

inline vec3& vec3::operator+=(const vec3 &v)
{
  e[0] += v.e[0];
  e[1] += v.e[1];
  e[2] += v.e[2];
  return *this;
}
inline vec3& vec3::operator-=(const vec3 &v)
{
  e[0] -= v.e[0];
  e[1] -= v.e[1];
  e[2] -= v.e[2];
  return *this;
}
inline vec3& vec3::operator*=(const vec3 &v)
{
  e[0] *= v.e[0];
  e[1] *= v.e[1];
  e[2] *= v.e[2];
  return *this;
}
inline vec3& vec3::operator/=(const vec3 &v)
{
  e[0] /= v.e[0];
  e[1] /= v.e[1];
  e[2] /= v.e[2];
  return *this;
}

inline vec3& vec3::operator*=(const float t)
{
  e[0] *= t;
  e[1] *= t;
  e[2] *= t;
  return *this;
}

inline vec3& vec3::operator/=(const float t)
{
  e[0] /= t;
  e[1] /= t;
  e[2] /= t;
  return *this;
}

inline vec3 unit_vector(vec3 v)
{
  return v / v.length();
}


/// @brief Generate random vector inside a unit sphere
vec3 random_in_unit_sphere()
{
  vec3 res;
  do
  {
    res = 2 * vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
  } while (res.squared_length() >= 1.0);
  return res;
}

/// @brief Reflect vector v along normal n
/// @param v vector
/// @param n normal
vec3 reflect(const vec3 &v, const vec3 &n)
{
  return v - (2 * dot(v,n) * n);
}

/// @brief Refract vector v along normal n with refractive index ratio r
///
/// Refraction is defined by n sin theta = n' sin theta'
/// To find the refracted vector we get 
/// theta = sin_inverse(sin theta * (n / n'))
/// @param v vector
/// @param n normal
/// @param r refractive index ratio
/// @param refracted (OUT) use refracted vector if returned true
/// @return true iff refraction is possible.
bool refract(const vec3 &v, const vec3 &n, float r, vec3 &refracted)
{
  vec3 uv = unit_vector(v);
  float dt = dot(uv, n);
  float discriminant = 1 - r * r * (1 - dt * dt);
  if (discriminant > 0)
  {
    refracted = r * (uv - (n * dt)) - n * sqrt(discriminant);
    return true;
  } else 
  {
    return false;
  }  
}

#endif