#ifndef RAY_H
#define RAY_H
#include "vec3.h"

class ray
{
public:
  ray() {}
  ray(const vec3 &origin, const vec3 &direction) : a(origin), b(direction) {}

  const vec3 &origin() const
  {
    return this->a;
  }

  const vec3 &direction() const
  {
    return this->b;
  }

  vec3 point_at_parameter(float t) const
  {
    return this->a + t * this->b;
  }

private:
  vec3 a, b;
};

#endif