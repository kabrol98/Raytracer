#ifndef UTILH
#define UTILH

#include "vec3.h"
#include "camera.h"
#include "assert.h"
/// Default values. 
#define IMG_RES 360
#define WIDESCREEN 16.0 / 9.0
#define IMG_HEIGHT IMG_RES
#define IMG_WIDTH IMG_HEIGHT * WIDESCREEN
#define IMG_SAMPLES 50
#define WORLD_SIZE 1
#define SPHERE_MAX 4
#define WITHIN(a,x,b) a <= x && x <= b
#define RANDF ((float)rand / RAND_MAX)

/// Constant color values
#define SKYBLUE       vec3(0.3,0.3,1)
#define WHITE         vec3(1,1,1)
#define BLACK         vec3(0,0,0)
#define RED           vec3(1,0,0)
#define GREEN         vec3(0,1,0)
#define BLUE          vec3(0,0,1)
#define GREYSCALE(x)  vec3(x,x,x)

/// Refractive index constants
#define GLASS_IDX   (1.5 + (drand48() * 0.2))
#define DIAMOND_IDX 2.4
#define AIR_IDX     1
#define HOLLOW_GLASS_IDX   -(1.5 + (drand48() * 0.2))
#define HOLLOW_DIAMOND_IDX -2.4
#define HOLLOW_AIR_IDX     -1

#ifndef NDEBUG
#   define ASSERT(condition, message) \
  do { \
    if (! (condition)) { \
      std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                << " line " << __LINE__ << ": " << message << std::endl; \
      std::terminate(); \
    } \
  } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

/// Configuration for image frame
typedef struct frame_ctx
{
  camera cam;
  size_t nX; /// horizontal frame resolution
  size_t nY; /// vertical frame resolution
  size_t nS; /// Anti-aliasing sample size
} frame_ctx;

/// Polynomial approximation for reflection probability.
float schlick(float cosine, float ref_idx)
{
  float r0 = (1 - ref_idx) / (1 + ref_idx);
  r0 *= r0;
  return r0 + (1-r0) * pow(1 - cosine, 5);
}

#endif