#ifndef SURFACE_H
#define SURFACE_H
#include "ray.h"
#include "hitpoint.h"
#include <memory>
#include <vector>

using namespace std;

namespace rt
{
class surface
{
public:
  virtual bool hit(const ray &r, float tmin, float tmax, float &t, hitpoint &hp) const = 0;
};
} // namespace rt

#endif