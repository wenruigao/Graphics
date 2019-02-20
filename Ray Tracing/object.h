#ifndef OBJECT_H
#define OBJECT_H
#include "surface.h"
#include "material.h"

namespace rt
{
class object
{
  public:
    object(shared_ptr<surface> surface, shared_ptr<material> frontm, shared_ptr<material> backm = nullptr)
        : s(surface), m1(frontm), m2(backm) {}

    bool hit(const ray &r, float tmin, float tmax, float &t, hitpoint &hp, shared_ptr<material> &m) const
    {
        if (!this->s->hit(r, tmin, tmax, t, hp))
        {
            return false;
        }

        m = nullptr;
        if (hp.side() == 0)
        {
            m = this->m1;
        }
        else
        {
            m = this->m2;
        }

        return m != nullptr;
    }

  private:
    shared_ptr<surface> s;
    shared_ptr<material> m1, m2;
};
} // namespace rt

#endif