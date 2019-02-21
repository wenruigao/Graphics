#ifndef MATERIAL_H
#define MATERIAL_H
#include "ray.h"
#include "hitpoint.h"

namespace rt
{
class material
{
public:
  virtual bool scatter(const ray &r_in, const hitpoint &hp, vec3 &attenuation, ray &scattered) const = 0;
  virtual vec3 emitted(float u, float v, const vec3 &p) const
  {
    return vec3(0.0f, 0.0f, 0.0f);
  }
};
} // namespace rt

#endif