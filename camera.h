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
    /// Default constructor, set at origin and use default frame values
    camera(): 
      origin(ORIGIN_DEFAULT),
      corner(CORNER_DEFAULT),
      horizontal(HORIZONTAL_DEFAULT),
      vertical(VERTICAL_DEFAULT) {}

    /// @brief FOV constructor
    ///
    /// Creates a frame through which the camera will view the scene
    /// given a field of vision and aspect ratio.
    /// @param vfov Field of vision in angles
    /// @param aspect ratio
    camera(float vfov, float aspect) 
    {
      float theta = vfov * (M_PI / 180);
      float half_height = tan(theta / 2);
      float half_width = half_height * aspect;
      /// Assume distance at 1.0, use height and width to define frame.
      corner = vec3(-half_width, -half_height, -1.0);
      vertical = vec3(0, 1, 0) * 2 * half_height;
      horizontal = vec3(1, 0, 0) * 2 * half_width;
      origin = vec3(0,0,0);

    }

    /// @brief Positional constructor
    ///
    /// Creates a (u,v,w) space given the origin, focus and up vector
    /// Allows caller to keep track of a target while moving through space
    /// @param lookfrom Origin vector for camera
    /// @param lookat Point vector on which to focus camera
    /// @param vup "up" vector will be projected onto the perpendicular plane to define camera rotation.
    /// @param vfov Field of vision in angles
    /// @param aspect ratio
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect) 
    {
      vec3 u,v,w; /// vectors to denote camera space.
      float theta = vfov * (M_PI / 180);
      float half_height = tan(theta / 2);
      float half_width = half_height * aspect;
      /// Define new unit vectors based on vector inputs
      w = unit_vector(lookfrom - lookat); /// the new 'Z' direction is from lookfrom towards lookat
      u = unit_vector(cross(vup, w)); /// the new 'X' direction is perpendicular to VUP and Z
      v = cross(w, u); /// This is vup projected onto the plane defined by w.

      std::cout << dot(v,u) << ", " << dot(w,v) << ", " << dot(w,u) << "\n";
      std::cout << w.length() << ", " << u.length() << ", " << v.length() << "\n";
      /// Use the same formula in the fov constructor, but project into the u,v,w space
      origin = lookfrom;
      /// Assume distance at 1.0, use height and width to define frame.
      corner = origin -(half_width * u) - (half_height * v) - (1.0 * w);
      vertical = 2 * half_height * v;
      horizontal = 2 * half_width * u;
    }
    
    
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