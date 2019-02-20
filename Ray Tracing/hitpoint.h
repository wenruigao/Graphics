#ifndef HITPOINT_H
#define HITPOINT_H
#include "vec3.h"
#include "material.h"
#include <memory>

using namespace std;

namespace rt
{
class hitpoint
{
public:
  hitpoint() {}
  hitpoint(const vec3 &point, const vec3 &normal, int side) : v0(point),
                                                              v1(normal),
                                                              s(side) {}

  inline const vec3 &point() const
  {
    return this->v0;
  }

  inline const vec3 &normal() const
  {
    return this->v1;
  }

  inline int side() const
  {
    return this->s;
  }

private:
  vec3 v0, v1;
  int s;
};
} // namespace rt

#endif