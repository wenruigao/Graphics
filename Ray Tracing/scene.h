#ifndef SCENE_H
#define SCENE_H
#include "object.h"

namespace rt
{
class scene
{
public:
  scene() {}

  bool hit(const ray &r, float tmin, float tmax, hitpoint &hp, shared_ptr<material> &m) const
  {
    bool hit_anything = false;
    float closest_so_far = tmax;
    for (auto object : this->objects)
    {
      float t;
      hitpoint hp0;
      shared_ptr<material> m0;
      if (object->hit(r, tmin, closest_so_far, t, hp0, m0))
      {
        hit_anything = true;
        closest_so_far = t;
        hp = hp0;
        m = m0;
      }
    }
    return hit_anything;
  }

  void add(shared_ptr<object> obj)
  {
    this->objects.push_back(obj);
  }

private:
  vector<shared_ptr<object>> objects;
};
} // namespace rt

#endif