#ifndef HITABLE_H
#define HITABLE_H

#include <stdlib.h>
#include <string.h>
#include "ray.h"
#include "assert.h"
#define DEFAULT_SIZE 8
using namespace std;

class material;

/* Records normal, position, light ray's t in order to compute textures. */
typedef struct hit_record
{
  float t;
  vec3 p;
  vec3 normal;
  material *mat;
} hit_record;

/* Parent class of hitable objects. Hit method overloaded by children.*/
class hitable
{
  public:
    /// Generic destructor
    virtual ~hitable() {}
    /// Generic hit function.
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
};

/* Stores a heap allocated list of hitable objects. Can be queried with a ray. */
class hit_list
{
  public:
    hit_list();
    ~hit_list();
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
  list = new hitable *[list_length];
}


/// @brief Destroy world objects and free memory.
/// @param world hitable list of heap allocated objects.
hit_list::~hit_list()
{
  for (int i = 0; i < size(); i ++)
  {
    delete list[i];
  }
  delete list;
}

/// @brief Push to hit list.
/// @param hitable object to add
void hit_list::push(hitable *h)
{
  if (list_size + 1 > list_length)
  {
    // Resize hitlist.
    list_length *= 2;
    hitable **new_list = new hitable *[list_length];
    /// memset to catch memory errors.
    memset(new_list, 0xff, list_length * sizeof( hitable *));
    /// Copy over hitable pointers to new list.
    memcpy(new_list, list, list_size * sizeof(hitable *));
    delete list;
    list = new_list;
  }
  // Add to list.
  list[list_size] = h;
  list_size++;
  // cout << "Added " << h << " to world, new list size " << list_size << "\n";
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
    // cout << i << " " <<  list_size << " " << list << '\n';
    hitable *h = list[i];
    // cout << "dereferenced " << h << "\n";
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