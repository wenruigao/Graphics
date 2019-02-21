#ifndef DIFFUSE_LIGHT_H
#define DIFFUSE_LIGHT_H
#include "material.h"
#include "texture.h"
#include <memory>

using namespace std;

namespace rt
{
class diffuse_light : public material
{
public:
  diffuse_light(shared_ptr<texture> emit) : emit(emit) {}

  virtual bool scatter(const ray &r_in, const hitpoint &hp, vec3 &attenuation, ray &scattered) const
  {
    return false;
  }

  virtual vec3 emitted(float u, float v, const vec3 &p) const
  {
    return this->emit->value(u, v, p);
  }

private:
  shared_ptr<texture> emit;
};
} // namespace rt

#endif