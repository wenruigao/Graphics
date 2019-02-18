#ifndef HITABLE_H
#define HITABLE_H
#include "ray.h"
#include "hitpoint.h"
#include <memory>
#include <vector>

using namespace std;

class hitable
{
public:
  virtual bool hit(const ray &r, float t_min, float t_max, float &t, hitpoint &hp) const = 0;
};

class hitable_list : public hitable
{
public:
  hitable_list(vector<shared_ptr<hitable>> hitables) : hitables(hitables) {}
  bool hit(const ray &r, float t_min, float t_max, float &t, hitpoint &hp) const
  {
    bool hit_anything = false;
    float closest_so_far = t_max;
    hitpoint hp0;
    for (size_t i = 0; i < hitables.size(); i++)
    {
      if (hitables[i]->hit(r, t_min, closest_so_far, t, hp0))
      {
        hit_anything = true;
        closest_so_far = t;
        hp = hp0;
      }
    }
    return hit_anything;
  }

private:
  vector<shared_ptr<hitable>> hitables;
};

#endif