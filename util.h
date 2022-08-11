#ifndef UTILH
#define UTILH

#include "vec3.h"

/// Default values. 
#define IMG_HEIGHT 400
#define IMG_WIDTH 800
#define WORLD_SIZE 1
#define SPHERE_MAX 4
#define WITHIN(a,x,b) a <= x && x <= b
#define RANDF ((float)rand / RAND_MAX)

/// Configuration for image frame
typedef struct frame_ctx
{
  vec3 camera; /// Camera position
  vec3 corner; /// top-left corner of iamge frame
  vec3 horizontal; /// horizontal frame length
  vec3 vertical; /// vertical frame length
  size_t nX; /// horizontal frame resolution
  size_t nY; /// vertical frame resolution
} frame_ctx;

#endif