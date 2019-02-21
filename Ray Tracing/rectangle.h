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
        this->v1 = p1 - p2;
        this->v2 = p3 - p2;
        this->normal = unit_vector(cross(this->v1, this->v2));
        this->d = -dot(this->normal, p2);
        this->u2 = dot(this->v1, this->v1);
        this->u4 = dot(this->v2, this->v2);
    }

    virtual bool hit(const ray &r, float tmin, float tmax, float &t, hitpoint &hp, bool &front) const
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
            vec3 point = r.point_at_parameter(t);
            vec3 v = point - this->p2;

            float u1 = dot(v, this->v1);
            float u3 = dot(v, this->v2);

            if (0 < u1 && 0 < u3 && u1 < this->u2 && u3 < this->u4)
            {
                if (a < 0)
                {
                    hp = hitpoint(point, this->normal, 1.0f - u1, 1.0f - u3);
                    front = true;
                }
                else
                {
                    hp = hitpoint(point, -this->normal, 1.0f - u1, 1.0f - u3);
                    front = false;
                }
                return true;
            }
        }

        return false;
    }

  private:
    vec3 p1, p2, p3, v1, v2, normal;
    float d, u2, u4;
};
} // namespace rt

#endif