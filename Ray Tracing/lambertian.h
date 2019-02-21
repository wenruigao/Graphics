#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H
#include "material.h"
#include "texture.h"

namespace rt
{
class lambertian : public material
{
public:
  lambertian(shared_ptr<texture> albedo) : albedo(albedo) {}

  virtual bool scatter(const ray &r_in, const hitpoint &hp, vec3 &attenuation, ray &scattered) const
  {
    vec3 target = hp.point() + hp.normal() + random_in_unit_sphere();
    attenuation = this->albedo->value(hp.u(), hp.v(), hp.point());
    scattered = ray(hp.point(), target - hp.point());
    return true;
  }

private:
  shared_ptr<texture> albedo;
};
} // namespace rt

#endif