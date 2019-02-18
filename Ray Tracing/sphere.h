#ifndef SPHERE_H
#define SPHERE_H
#include "hitable.h"
#include "material.h"

class sphere : public hitable
{
  public:
    sphere(const vec3 &center, float radius, shared_ptr<material> material) : center(center), radius(radius), material(material) {}
    virtual bool hit(const ray &r, float tmin, float tmax, float &t, hitpoint &hp) const
    {
        vec3 oc = r.origin() - this->center;
        float a = dot(r.direction(), r.direction());
        float b = dot(oc, r.direction());
        float c = dot(oc, oc) - this->radius * this->radius;
        float discriminant = b * b - a * c;
        if (discriminant > 0)
        {
            t = (-b - sqrtf(discriminant)) / (a);
            if (t < tmax && t > tmin)
            {
                hp.point = r.point_at_parameter(t);
                hp.normal = unit_vector(hp.point - center);
                hp.material = this->material;
                return true;
            }

            t = (-b + sqrtf(discriminant)) / (a);
            if (t < tmax && t > tmin)
            {
                hp.point = r.point_at_parameter(t);
                hp.normal = unit_vector(hp.point - center);
                hp.material = this->material;
                return true;
            }
        }

        return false;
    }

  private:
    vec3 center;
    float radius;
    shared_ptr<material> material;
};
#endif