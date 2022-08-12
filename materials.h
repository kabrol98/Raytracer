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
      /// Set attenuation to white.
      attenuation = WHITE;

      /// Compute outward normal, cosine, refractive ratio for refraction
      vec3 outward_normal, refracted;
      float ref_ratio, cosine, reflect_prob;
      if (dot(r_in.direction(), rec.normal) > 0)
      {
        outward_normal = -rec.normal;
        ref_ratio = ref_idx;
        /// TODO: Relearn trigonimetry to understand this math
        // cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
        cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
        cosine = sqrt(1 - ref_idx*ref_idx*(1-cosine*cosine));
      } else 
      {
        outward_normal = rec.normal;
        ref_ratio = 1.0 / ref_idx;
        cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
      }
      /// Define reflection probabililty based on whether refraction is possible
      if (refract(r_in.direction(), outward_normal, ref_ratio, refracted))
      {
        reflect_prob = schlick(cosine, ref_idx);
        ASSERT(WITHIN(0,reflect_prob, 1), "Reflection probability out of bounds!\n");
      } else {
        reflect_prob = 1.0;
      }

      if (drand48() < reflect_prob)
      {
        scattered = ray(rec.p, reflected);
        // return true;
      } else 
      {
        scattered = ray(rec.p, refracted);
      }
      return true;
    }

  private:
    float ref_idx; /// Refractive index
};


#endif