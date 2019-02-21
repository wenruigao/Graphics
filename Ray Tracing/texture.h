#ifndef TEXTURE_H
#define TEXTURE_H
#include "vec3.h"

namespace rt
{
class texture
{
  public:
    virtual vec3 value(float u, float v, const vec3 &p) const = 0;
};
} // namespace rt

#endif