#ifndef SPHERE_H
#define SPHERE_H
#include "surface.h"

namespace rt
{

static void get_sphere_uv(const vec3 &p, float &u, float &v)
{
    float phi = atan2f(p.z(), p.x());
    float theta = asinf(p.y());
    u = 1.0f - (phi + M_PI) / (2.0f * M_PI);
    v = (theta + M_PI_2) / M_PI;
}

class sphere : public surface
{
  public:
    sphere(const vec3 &center, float radius) : center(center), radius(radius) {}

    virtual bool hit(const ray &r, float tmin, float tmax, float &t, hitpoint &hp, bool &front) const
    {
        vec3 oc = r.origin() - this->center;
        float a = dot(r.direction(), r.direction());
        float b = dot(oc, r.direction());
        float c = dot(oc, oc) - this->radius * this->radius;
        float discriminant = b * b - a * c;

        if (discriminant > 0)
        {
            t = (-b + copysignf(sqrtf(discriminant), -c)) / a;

            if (t < tmax && t > tmin)
            {
                auto point = r.point_at_parameter(t);
                auto normal = (point - this->center) / this->radius;
                float u, v;
                get_sphere_uv(normal, u, v);

                if (c > 0)
                {
                    hp = hitpoint(point, normal, u, v);
                    front = true;
                }
                else
                {
                    hp = hitpoint(point, -normal, u, v);
                    front = false;
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