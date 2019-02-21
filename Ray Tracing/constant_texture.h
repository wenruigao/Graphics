#ifndef CONSTANT_TEXTURE_H
#define CONSTANT_TEXTURE_H
#include "texture.h"

namespace rt
{
class constant_texture : public texture
{
public:
  constant_texture(vec3 color) : color(color) {}

  virtual vec3 value(float u, float v, const vec3 &p) const
  {
    return this->color;
  }

private:
  vec3 color;
};
} // namespace rt

#endif