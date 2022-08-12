#ifndef CAMERAH
#define CAMERAH

#define ORIGIN_DEFAULT vec3(0, 0, 0)
#define CORNER_DEFAULT vec3(-2, -1, -1)
#define HORIZONTAL_DEFAULT vec3(4, 0, 0)
#define VERTICAL_DEFAULT vec3(0, 2, 0)

#include "ray.h"

/// Camera class
/// Encapsulates origin of light rays and frame dimensions
/// Camera emits light rays for each pixel in the image, and 
/// its direction is determined by the frame dimensions. This will go from
/// the top left corner til the horizontal/ vertical dimensions.
class camera
{
  public:
    camera(): 
      origin(ORIGIN_DEFAULT),
      corner(CORNER_DEFAULT),
      horizontal(HORIZONTAL_DEFAULT),
      vertical(VERTICAL_DEFAULT) {}
    camera(
      vec3 o,
      vec3 c,
      vec3 h,
      vec3 v
    ): origin(o), corner(c), horizontal(h), vertical(v) {}
    
    ray get_ray(float u, float v)
    {
      return ray(origin, corner + (u * horizontal) + (v * vertical) - origin);
    }

  private:
    vec3 origin; // Camera origin
    vec3 corner; // Top left corner of frame
    vec3 horizontal; // Width of frame
    vec3 vertical; // Height of frame
};

#endif