#ifndef HITPOINT_H
#define HITPOINT_H
#include "vec3.h"
#include <memory>

using namespace std;

namespace rt
{
class hitpoint
{
public:
  hitpoint() {}
  hitpoint(const vec3 &point, const vec3 &normal, float u, float v)
      : v0(point),
        v1(normal),
        u0(u),
        u1(v) {}

  inline const vec3 &point() const
  {
    return this->v0;
  }

  inline const vec3 &normal() const
  {
    return this->v1;
  }

  inline float u() const
  {
    return this->u0;
  }

  inline float v() const
  {
    return this->u1;
  }

private:
  vec3 v0, v1;
  float u0, u1;
};
} // namespace rt

#endif