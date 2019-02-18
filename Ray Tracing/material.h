#ifndef MATERIAL_H
#define MATERIAL_H
#include "ray.h"

class material
{
public:
  virtual bool scatter(const ray &r_in, const vec3 &point, const vec3 &normal, vec3 &attenuation, ray &scattered) const = 0;
};

#endif