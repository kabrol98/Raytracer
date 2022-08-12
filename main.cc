#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include "ray.h"
#include "vec3.h"
#include "sphere.h"
#include "hitable.h"
#include "util.h"
#include "camera.h"
#include "materials.h"
#include "float.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

/// @brief return pixel color by querying world for a given light ray.
/// @param r incoming light ray
/// @param world list of hitable objects that produce colors when hit by the light ray.
/// @param depth used to ensure light rays don't scatter infinitely.
vec3 color(const ray &r, const hit_list *world, int depth)
{
  // Initialize hit record.
  hit_record rec;
  // Compute hitpoint.
  bool hit = world->hit(r, 0.0001, MAXFLOAT, rec);
  if (hit)
  {
    /// Attempt to scatter light ray given depth required.
    ray scattered;
    vec3 attenuation;
    if (depth < 50 && rec.mat->scatter(r, rec, attenuation, scattered))
    {
      /// Scatter light ray according to material recorded in hit record.
      return attenuation * color(scattered, world, depth + 1);
    } else 
    {
      /// Max depth was exceeded, or light was absorbed! 
      /// Return black pixel to signify shadow.
      return vec3(0,0,0);
    }
  } else
  {
    // Background compute.
    vec3 uv = unit_vector(r.direction()); 
    float t = 0.5 * (uv.y() + 1);
    /// Evenly blend sky blue and white along the ray direction's y axis.
    return (1 - t) * WHITE + (t) * SKYBLUE;
  }
}

/// @brief Generate heap allocated pixel map given hitable list and frame ctx
/// @param world List of objects to populate vector space.
/// @param frame frame context
vec3 **generate_image(
  hit_list *world,
  frame_ctx &frame)
{
  int nX = frame.nX;
  int nY = frame.nY;

  /// Allocate image column buffer.
  vec3 ** image = (vec3 **) malloc(nX);
  /// memset to track memory
  memset(image, 0xff, nX);
  for (int i = 0; i < nX; i ++)
  {
    /// Allocate image row buffer.
    image[i] = (vec3 *) malloc(sizeof(vec3) * nY);
    for (int j = 0; j < nY; j ++)
    {
      /// Sample light rays with slight variance
      /// Generate light ray from camera to frame position.
      vec3 pixel(0,0,0);
      for (int s = 0; s < frame.nS; s++)
      {
        float u = (float(i) + drand48()) / float(nX);
        float v = (float(j) + drand48()) / float(nY);
        ray light = frame.cam.get_ray(u,v);
        /// Send light ray into world, generate pixel value.
        pixel += color(light, world, 0);
      }
      pixel /= frame.nS;
      /// Gamma correct pixel value by taking the square root.
      pixel = pixel.sqrt3();
      
      /// Assign pixel value to image matrix.
      ASSERT(WITHIN(0,pixel.r(),1), "Pixel " << pixel << " out of bounds!");
      ASSERT(WITHIN(0,pixel.r(),1), "Pixel " << pixel << " out of bounds!");
      ASSERT(WITHIN(0,pixel.g(),1), "Pixel " << pixel << " out of bounds!");
      ASSERT(WITHIN(0,pixel.b(),1), "Pixel " << pixel << " out of bounds!");
      image[i][j] = pixel;
    }
  }
  return image;
}

/// @brief Initialize frame context with default values
/// @param Frame ctx reference
void initialize_frame(frame_ctx &frame)
{
  /// Define nX and nY by applying resolution to the aspect ratio.
  frame.nY = IMG_RES;
  frame.nX = IMG_RES * WIDESCREEN;
  frame.nS = IMG_SAMPLES;
  /// Define lookfrom, lookat, vup to position and rotate camera.
  vec3 lookfrom(-2,2,1);
  vec3 lookat(0,0,-1);
  vec3 vup (1,1,0);
  frame.cam = camera(
    lookfrom,
    lookat,
    vup,
    90, 
    WIDESCREEN);
  return;
}

/// @brief Generate world
///
/// Generate a list of hitable objects to display within frame.
/// @param frame Frame context for frame limits. 
hit_list *generate_world(const frame_ctx &frame)
{
  hit_list *world = new hit_list();

  /// TODO: Add more objects to hit list. 
  vec3 center = vec3(0,0,-2);
  float radius = 0.8;
  // material * matte_green = new lambertian(GREEN);
  // material * matte_red = new lambertian(red);
  /// Add matte green "planet" sphere below frame.
  world->push(
    new sphere(
      vec3(0, -(100 + radius), -1), 
      100,
      new lambertian(GREEN)
    ));
  /// Add matte red sphere front and center
  world->push(
    (hitable *) new sphere(
      center, 
      radius,
      new lambertian(RED)
    ));
  /// Add metal blue sphere to the left
  world->push(
    (hitable *) new sphere(
      center - vec3(2 * radius,0,0), 
      radius,
      new metal(SKYBLUE)
    ));
  /// Add glass sphere to the right
  world->push(
    (hitable *) new sphere(
      center + vec3(2 * radius,0,0), 
      radius,
      new dielectric(DIAMOND_IDX)
    ));
  

  return world;
}

/// @brief Destroy heap allocated image buffer. 
void destroy_image(vec3 **image, const frame_ctx &frame)
{
  int nX = frame.nX;
  vec3 *row;
  for (int i = 0; i < nX; i ++)
  {
    row = image[i];
    free(row);
    row = NULL;
  }
  free(image);
}

/// @brief Write image buffer to .ppm file
/// @param filename if non-null, writes to "file.ppm"
/// @param image pixel matrix
/// @param frame frame context
int write_ppm(char * filename, vec3 **image, const frame_ctx &frame)
{
  ofstream os;
  char default_filename[] = "file.ppm";
  if (!filename)
  {
    filename = default_filename;
  }
  os.open(filename);

  int ir, ig, ib;
  os << "P3\n" << frame.nX << " " << frame.nY << " " << "\n255\n";
  for (int j = frame.nY-1; j >= 0; j--)
  {
    for (int i = 0; i < frame.nX; i++)
    {
      vec3 pixel = image[i][j];
      ir = int(255.99 * pixel.r());
      ig = int(255.99 * pixel.g());
      ib = int(255.99 * pixel.b());
      os << ir << " " << ig << " " << ib << "\n";
    }
  }  
  return 0;
}

int main(int c, char **argv) 
{
  frame_ctx frame;
  /// Initialize frame TODO: Allow for parameter to change frame options
  initialize_frame(frame);
  /// Generate world of hitable objects
  hit_list *world = generate_world(frame);
  /// Generate 2-D pixel matrix of frame
  vec3 ** image = generate_image(world, frame);
  /// Write generated image to ppm file TODO: support other file types.
  write_ppm(NULL, image, frame);
  /// Destroy objects, free memory
  delete world;
  destroy_image(image, frame);

  return 0;
}