#ifndef DIELECTRIC_H
#define DIELECTRIC_H
#include "material.h"

static float schlick(float cosine, float ref_idx)
{
    float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * powf((1.0f - cosine), 5.0f);
}

class dielectric : public material
{
  public:
    dielectric(float ri) : ref_idx(ri), gen(random_device()()), dis(0.0f, 1.0f) {}

    virtual bool scatter(const ray &r_in, const vec3 &point, const vec3 &normal, vec3 &attenuation, ray &scattered) const
    {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), normal);
        float ni_over_nt;
        attenuation = vec3(1.0f, 1.0f, 1.0f);
        vec3 refracted;
        float cosine;
        if (dot(r_in.direction(), normal) > 0)
        {
            outward_normal = -normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.direction(), normal) / r_in.direction().length();
        }
        else
        {
            outward_normal = normal;
            ni_over_nt = 1.0f / ref_idx;
            cosine = -dot(r_in.direction(), normal) / r_in.direction().length();
        }

        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
        {
            float reflect_prob = schlick(cosine, ref_idx);
            if (dis(gen) >= reflect_prob)
            {
                scattered = ray(point, refracted);
                return true;
            }
        }

        scattered = ray(point, reflected);
        return true;
    }

  private:
    float ref_idx;
    mutable mt19937 gen;
    mutable uniform_real_distribution<float> dis;
};

#endif