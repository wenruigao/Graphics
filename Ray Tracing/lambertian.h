#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H
#include "material.h"

namespace rt
{
class lambertian : public material
{
public:
  lambertian(const vec3 &albedo) : albedo(albedo) {}

  virtual bool scatter(const ray &r_in, const vec3 &point, const vec3 &normal, vec3 &attenuation, ray &scattered) const
  {
    vec3 target = point + normal + random_in_unit_sphere();
    attenuation = albedo;
    scattered = ray(point, target - point);
    return true;
  }

private:
  vec3 albedo;
};
} // namespace rt

#endif