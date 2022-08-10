#include <iostream>
#include <fstream>
#include <algorithm>
#include <assert.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ray.h"
#include "vec3.h"
using namespace std;
#define PIX_HEIGHT 400
#define PIX_WIDTH 800
#define WITHIN(a,x,b) a <= x && x <= b

vec3 color_from_ray(const ray &r)
{
  vec3 uv =unit_vector(r.direction());
  float t = 0.5 * (uv.y() + 1);
  vec3 skyblue(.5, .5, 1);
  vec3 white(1, 1, 1);
  return (1 - t) * skyblue + (t) * white;
}


int write_ppm(char * filename)
{
  ofstream os;
  char default_filename[] = "file.ppm";
  if (!filename)
  {
    filename = default_filename;
  }
  os.open(filename);

  int nx = PIX_WIDTH;
  int ny = PIX_HEIGHT;
  int ir, ig, ib;
  vec3 lower_left_corner(-2, -1, -1);
  vec3 horizontal(4, 0, 0);
  vec3 vertical(0, 2, 0);
  vec3 origin(0, 0, 0);
  std::cout << "P3\n" << nx << " " << ny << " " << "\n255\n";
  for (int j = ny-1; j >= 0; j--)
  {
    for (int i = 0; i < nx; i++)
    {
      float u = float(i) / float(nx);
      float v = float(j) / float(ny);
      vec3 direction = lower_left_corner + (u * horizontal) + (v * vertical);
      ray cam_ray(origin, direction);
      vec3 pixel = color_from_ray(cam_ray);
      assert (WITHIN(0,pixel.r(),1));
      assert (WITHIN(0,pixel.g(),1));
      assert (WITHIN(0,pixel.b(),1));
      ir = int(255.99 * pixel.r());
      ig = int(255.99 * pixel.g());
      ib = int(255.99 * pixel.b());
      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }  
  return 0;
}

int main(int c, char **argv) 
{
  write_ppm(NULL);
  return 0;
}