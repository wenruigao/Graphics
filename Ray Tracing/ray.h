#ifndef RAY_H
#define RAY_H
#include "vec3.h"

namespace rt
{
class ray
{
public:
  ray() {}
  ray(const vec3 &origin, const vec3 &direction) : v0(origin), v1(direction) {}

  inline const vec3 &origin() const
  {
    return this->v0;
  }

  inline const vec3 &direction() const
  {
    return this->v1;
  }

  inline vec3 point_at_parameter(float t) const
  {
    return this->v0 + t * this->v1;
  }

private:
  vec3 v0, v1;
};
} // namespace rt

#endif