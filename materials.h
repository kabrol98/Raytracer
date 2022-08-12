#ifndef MATERIALSH
#define MATERIALSH

#include "ray.h"
#include "util.h"
#include "hitable.h"

/// Material Class
/// Material children will expose a "scatter function", which will tell the caller
/// how to handle a ray hitting an object.
class material
{
  public:
    virtual ~material() {}
    virtual bool scatter(const ray &r_in, hit_record &rec, vec3 &attenuation, ray &scatter) const = 0;
};

class lambertian: public material
{
  public:
    virtual ~lambertian() {}
    lambertian(const vec3 &ab): albedo(ab) {} 
    virtual bool scatter(const ray &r_in, hit_record &rec, vec3 &attenuation, ray &scattered) const
    {
      /// scatter the incoming ray randonmly using a unit sphere tangent to the hitpoint.
      vec3 target = rec.p + rec.normal + random_in_unit_sphere();
      scattered = ray(rec.p, target - rec.p);
      attenuation = albedo;
      return true;

    }
  private:
    vec3 albedo;
};

class metal: public material
{
  public:
    virtual ~metal () {}
    metal(const vec3 &ab): albedo(ab), fuzz(0) {} 
    metal(const vec3 &ab, float f): albedo(ab), fuzz(f) {} 
    virtual bool scatter(const ray &r_in, hit_record &rec, vec3 &attenuation, ray &scattered) const
    {
      /// Reflect the incoming ray along the hitpoint's normal axis.
      vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
      scattered = ray(rec.p, reflected + (fuzz * random_in_unit_sphere()));
      attenuation = albedo;
      return true;
    }
  private:
    vec3 albedo;
    float fuzz;
};

class dielectric : public material
{
  public:
    virtual ~dielectric () {}
    dielectric(): ref_idx(1) {}
    dielectric(float ref): ref_idx(ref) {}
    virtual bool scatter(const ray &r_in, hit_record &rec, vec3 &attenuation, ray &scattered) const
    {
      /// Generate reflected ray
      vec3 reflected = reflect(r_in.direction(), rec.normal);
      scattered = ray(rec.p,reflected); return false;
      /// Compute outward normal and refractive ratio for refraction
      vec3 outward_normal, refracted;
      /// Set attenuation to white.
      attenuation = WHITE;
      float ref_ratio;
      if (dot(r_in.direction(), rec.normal) > 0)
      {
        outward_normal = -rec.normal;
        ref_ratio = ref_idx;
      } else 
      {
        outward_normal = rec.normal;
        ref_ratio = 1.0 / ref_idx;
      }
      /// Refract, return refracted vs. reflected ray based on discriminant
      if (refract(r_in.direction(), outward_normal, ref_ratio, refracted))
      {
        scattered = ray(rec.p, refracted);
        return true;
      } else 
      {
        scattered = ray(rec.p, reflected);
        return false;
      }
    }

  private:
    float ref_idx; /// Refractive index
};


#endif