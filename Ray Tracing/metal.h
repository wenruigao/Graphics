#ifndef MATAL_H
#define MATAL_H
#include "material.h"

namespace rt
{
class metal : public material
{
public:
  metal(const vec3 &albedo, float fuzz) : albedo(albedo), fuzz(min(fuzz, 1.0f)) {}

  virtual bool scatter(const ray &r_in, const vec3 &point, const vec3 &normal, vec3 &attenuation, ray &scattered) const
  {
    vec3 reflected = reflect(unit_vector(r_in.direction()), normal);
    attenuation = albedo;
    scattered = ray(point, reflected + fuzz * random_in_unit_sphere());
    return dot(scattered.direction(), normal) > 0;
  }

private:
  vec3 albedo;
  float fuzz;
};
} // namespace rt

#endif