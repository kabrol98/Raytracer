#include <iostream>
#include <fstream>
#include <algorithm>
#include <assert.h>
#include <stdlib.h>
#include "ray.h"
#include "vec3.h"
#include "sphere.h"
#include "hitable.h"
#include "util.h"
#include "float.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

vec3 color(const ray &r, const hit_list &world)
{
  // Standard colors.
  vec3 skyblue(.5, .5, 1);
  vec3 white(1, 1, 1);
  vec3 red(1,0.5,0.5);

  // Background compute.
  vec3 uv = unit_vector(r.direction());
  float t = 0.5 * (uv.y() + 1);

  // Initialize hit record.
  hit_record rec;
  // Compute hitpoint.
  bool hit = world.hit(r, 0, MAXFLOAT, rec);
  if (hit)
  {
    // Compute normal using color map. 
    return 0.5 * (rec.normal + vec3(1,1,1));
  } else
  {
    return (1 - t) * white + (t) * skyblue;
  }
}

/// @brief Generate heap allocated pixel map given hitable list and frame ctx
/// @param world List of objects to populate vector space.
/// @param frame frame context
vec3 **generate_image(
  hit_list world,
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
      /// Generate light ray from camera to frame position.
      float u = float(i) / float(nX);
      float v = float(j) / float(nY);
      vec3 direction = frame.corner + (u * frame.horizontal) + (v * frame.vertical);
      ray light(frame.camera, direction);
      /// Send light ray into world, generate pixel value.
      vec3 pixel = color(light, world);
      /// Assign pixel value to image matrix.
      assert (WITHIN(0,pixel.r(),1));
      assert (WITHIN(0,pixel.g(),1));
      assert (WITHIN(0,pixel.b(),1));
      image[i][j] = pixel;
    }
  }
  return image;
}

/// @brief Initialize frame context with default values
/// @param Frame ctx reference
void initialize_frame(frame_ctx &frame)
{
  frame.corner = vec3(-2, -1, -1);
  frame.horizontal = vec3(4, 0, 0);
  frame.vertical = vec3(0, 2, 0);
  frame.camera = vec3(0, 0, 0);
  frame.nX = IMG_WIDTH;
  frame.nY = IMG_HEIGHT;
  return;
}

/// @brief Generate world
///
/// Generate a list of hitable objects to display within frame.
/// @param frame Frame context for frame limits. 
hit_list generate_world(const frame_ctx &frame)
{
  hit_list world = hit_list();

  /// TODO: Add more objects to hit list. 
  vec3 center = vec3(0,0,-2);
  float radius = 0.5;
  world.push((hitable *) new sphere(center, radius));

  return world;
}

/// @brief Destroy world objects and free memory.
/// @param world hitable list of heap allocated objects.
void destroy_world(const hit_list &world)
{
  for (int i = 0; i < world.size(); i ++)
  {

  }
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
  hit_list world = generate_world(frame);
  /// Generate 2-D pixel matrix of frame
  vec3 ** image = generate_image(world, frame);
  /// Write generated image to ppm file TODO: support other file types.
  write_ppm(NULL, image, frame);
  /// Destroy objects, free memory
  destroy_world(world);
  destroy_image(image, frame);

  return 0;
}