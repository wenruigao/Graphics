#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "surface.h"

namespace rt
{
class rectangle : public surface
{
  public:
    rectangle(const vec3 &p1, const vec3 &p2, const vec3 &p3) : p1(p1),
                                                                p2(p2),
                                                                p3(p3)
    {
        vec3 v1 = p1 - p2;
        vec3 v2 = p3 - p2;
        this->normal = unit_vector(cross(v1, v2));
        this->d = -dot(this->normal, p2);
    }

    virtual bool hit(const ray &r, float tmin, float tmax, float &t, hitpoint &hp) const
    {
        float b = dot(this->normal, r.direction());

        if (b == 0)
        {
            return false;
        }

        float a = -dot(this->normal, r.origin()) - this->d;
        t = a / b;
        if (t < tmax && t > tmin)
        {
            vec3 p = r.point_at_parameter(t);

            vec3 v1 = this->p1 - this->p2;
            vec3 v2 = this->p3 - this->p2;
            vec3 v = p - this->p2;
            float u1 = dot(v, v1);
            float u2 = dot(v1, v1);
            float u3 = dot(v, v2);
            float u4 = dot(v2, v2);

            if (0 < u1 && 0 < u3 && u1 < u2 && u3 < u4)
            {
                if (a < 0)
                {
                    hp = hitpoint(p, this->normal, 0);
                }
                else
                {
                    hp = hitpoint(p, -this->normal, 1);
                }
                return true;
            }
        }

        return false;
    }

  private:
    vec3 p1, p2, p3, normal;
    float d;
};
} // namespace rt

#endif