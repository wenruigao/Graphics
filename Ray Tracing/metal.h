#ifndef MATAL_H
#define MATAL_H
#include "material.h"

class metal : public material
{
public:
  metal(const vec3 &albedo, float fuzz) : albedo(albedo), fuzz(min(fuzz, 1.0f)) {}

  virtual bool scatter(const ray &r_in, const vec3 &point, const vec3 &normal, vec3 &attenuation, ray &scattered) const
  {
    vec3 reflected = reflect(unit_vector(r_in.direction()), normal);
    scattered = ray(point, reflected + fuzz * random_unit_vector());
    attenuation = albedo;
    return dot(scattered.direction(), normal) > 0;
  }

private:
  vec3 albedo;
  float fuzz;
};

#endif