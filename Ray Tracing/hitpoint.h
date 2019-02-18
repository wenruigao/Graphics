#ifndef HITPOINT_H
#define HITPOINT_H
#include "vec3.h"
#include "material.h"
#include <memory>

using namespace std;

struct hitpoint
{
  vec3 point;
  vec3 normal;
  shared_ptr<material> material;
};

#endif