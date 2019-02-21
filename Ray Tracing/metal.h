#ifndef MATAL_H
#define MATAL_H
#include "material.h"
#include "texture.h"

namespace rt
{
class metal : public material
{
public:
  metal(shared_ptr<texture> albedo, float fuzz) : albedo(albedo), fuzz(min(fuzz, 1.0f)) {}

  virtual bool scatter(const ray &r_in, const hitpoint &hp, vec3 &attenuation, ray &scattered) const
  {
    vec3 reflected = reflect(unit_vector(r_in.direction()), hp.normal());
    attenuation = this->albedo->value(hp.u(), hp.v(), hp.point());
    scattered = ray(hp.point(), reflected + fuzz * random_in_unit_sphere());
    return dot(scattered.direction(), hp.normal()) > 0;
  }

private:
  shared_ptr<texture> albedo;
  float fuzz;
};
} // namespace rt

#endif