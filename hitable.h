#ifndef HITABLE_H
#define HITABLE_H

#include <stdlib.h>
#include <string.h>
#include "ray.h"
#define DEFAULT_SIZE 8
typedef struct hit_record
{
  float t;
  vec3 p;
  vec3 normal;
} hit_record;

class hitable
{
  public:
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
};

class hit_list
{
  public:
    hit_list();
    void push(const hitable *h);
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec);
    
  private:
    hitable **list;
    int list_size;
    int list_length;
};

hit_list::hit_list()
{
  list_size = 0;
  list_length = DEFAULT_SIZE;
  list = (hitable **) malloc(list_length);
}

void hit_list::push(const hitable *h)
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

bool hit_list::hit(const ray &r, float t_min, float t_max, hit_record &rec)
{

}

#endif