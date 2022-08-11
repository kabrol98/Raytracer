#ifndef HITABLE_H
#define HITABLE_H

#include <stdlib.h>
#include <string.h>
#include "ray.h"
#include "assert.h"
#define DEFAULT_SIZE 8

/* Records normal, position, light ray's t in order to compute textures. */
typedef struct hit_record
{
  float t;
  vec3 p;
  vec3 normal;
} hit_record;

/* Parent class of hitable objects. Hit method overloaded by children.*/
class hitable
{
  public:
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
};

/* Stores a heap allocated list of hitable objects. Can be queried with a ray. */
class hit_list
{
  public:
    hit_list();
    void push(hitable *h);
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
    inline hitable * get(int i) const  { assert(i< list_size); return list[i]; }
    inline int size() const  { return list_size; }
    inline void destroy() {free(list); list = NULL; list_size = list_length = 0; }
    
  private:
    hitable **list;   /* Heap allocated list of hitable pointers. */
    int list_size;    /* Number of objects in list. */
    int list_length;  /* Current allocated length for list. */
};

/* Initialize hit list. */
hit_list::hit_list()
{
  list_size = 0;
  list_length = DEFAULT_SIZE;
  list = (hitable **) malloc(list_length);
}

/// @brief Push to hit list.
/// @param hitable object to add
void hit_list::push(hitable *h)
{
  if (list_size + 1 > list_length)
  {
    // Resize hitlist.
    list_length *= 2;
    hitable **new_list = (hitable **) malloc(list_length);
    memcpy(new_list, list, list_size);
    free(list);
    list = new_list;
  }
  // Add to list.
  list[list_size] = h;
  list_size++;
}

/// @brief Iterate over object list, check if an object was hit
/// @param r: (IN) light ray
/// @param t_min (IN) minimum distance for which to compute intersections
/// @param t_max (IN) maximum distance for which to compute intersections
/// @param rec (OUT) Record t, normal, point of intersection
/// @return true iff an object was hit by ray r and update hit record.
bool hit_list::hit(const ray &r, float t_min, float t_max, hit_record &rec) const
{
  float t_closest = t_max;
  bool did_hit =  false;
  hit_record temp_record;
  for (int i = 0; i < list_size; i ++)
  {
    hitable *h = list[i];
    if (h->hit(r, t_min, t_closest, temp_record))
    {
      did_hit = true;
      rec = temp_record;
      t_closest = rec.t;
    }
  }
  return did_hit;
}

#endif