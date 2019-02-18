#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H
#include "material.h"

class lambertian : public material
{
public:
  lambertian(const vec3 &albedo) : albedo(albedo) {}
  virtual bool scatter(const ray &r_in, const vec3 &point, const vec3 &normal, vec3 &attenuation, ray &scattered) const
  {
    vec3 target = point + normal + random_in_unit_sphere();
    scattered = ray(point, target - point);
    attenuation = albedo;
    return true;
  }

private:
  vec3 albedo;
};

#endif