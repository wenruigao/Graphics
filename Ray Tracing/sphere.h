#ifndef SPHERE_H
#define SPHERE_H
#include "surface.h"

namespace rt
{
class sphere : public surface
{
  public:
    sphere(const vec3 &center, float radius) : center(center), radius(radius) {}

    virtual bool hit(const ray &r, float tmin, float tmax, float &t, hitpoint &hp) const
    {
        vec3 oc = r.origin() - this->center;
        float a = dot(r.direction(), r.direction());
        float b = dot(oc, r.direction());
        float c = dot(oc, oc) - this->radius * this->radius;
        float discriminant = b * b - a * c;

        if (discriminant > 0)
        {
            if (c > 0) //outside the sphere
            {
                t = (-b - sqrtf(discriminant)) / (a);
            }
            else //inside or on the sphere
            {
                t = (-b + sqrtf(discriminant)) / (a);
            }

            if (t < tmax && t > tmin)
            {
                auto p = r.point_at_parameter(t);
                if (c > 0)
                {
                    hp = hitpoint(p, (p - center) / this->radius, 0);
                }
                else
                {
                    hp = hitpoint(p, (center - p) / this->radius, 1);
                }
                return true;
            }
        }

        return false;
    }

  private:
    vec3 center;
    float radius;
};
} // namespace rt

#endif