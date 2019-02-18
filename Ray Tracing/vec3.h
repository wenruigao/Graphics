#ifndef VEC3_H
#define VEC3_H
#include <cmath>
#include <random>

using namespace std;

class vec3
{
  public:
    vec3() {}
    vec3(float e0, float e1, float e2) : e{e0, e1, e2} {}
    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    inline float operator[](int i) const
    {
        return e[i];
    }

    inline float squared_length() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    inline float length() const
    {
        return sqrtf(this->squared_length());
    }

    inline vec3 operator-() const
    {
        return vec3(-this->e[0], -this->e[1], -this->e[2]);
    }

    inline vec3 &operator+=(const vec3 &v)
    {
        this->e[0] += v[0];
        this->e[1] += v[1];
        this->e[2] += v[2];
        return *this;
    }

    inline vec3 &operator/=(float t)
    {
        this->e[0] /= t;
        this->e[1] /= t;
        this->e[2] /= t;
        return *this;
    }

  private:
    float e[3];
};

inline vec3 operator+(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

inline vec3 operator*(float t, const vec3 &v)
{
    return vec3(v[0] * t, v[1] * t, v[2] * t);
}

inline vec3 operator*(const vec3 &v, float t)
{
    return vec3(v[0] * t, v[1] * t, v[2] * t);
}

inline vec3 operator/(const vec3 &v, float t)
{
    return vec3(v[0] / t, v[1] / t, v[2] / t);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]);
}

inline vec3 unit_vector(const vec3 &v)
{
    return v / v.length();
}

inline float dot(const vec3 &v1, const vec3 &v2)
{
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2)
{
    return vec3(
        v1[1] * v2[2] - v1[2] * v2[1],
        v1[2] * v2[0] - v1[0] * v2[2],
        v1[0] * v2[1] - v1[1] * v2[0]);
}

inline vec3 reflect(const vec3 &v, const vec3 &n)
{
    return v - 2 * dot(v, n) * n;
}

inline bool refract(const vec3 &v, const vec3 &n, float ni_over_nt, vec3 &refracted)
{
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);
    if (discriminant > 0)
    {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrtf(discriminant);
        return true;
    }
    else
    {
        return false;
    }
}

inline vec3 random_in_unit_sphere()
{
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_real_distribution<float> dis(-1.0f, 1.0f);

    vec3 v;
    do
    {
        v = vec3(dis(gen), dis(gen), dis(gen));
    } while (v.squared_length() >= 1.0f);
    return v;
}

inline vec3 random_in_unit_disk()
{
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_real_distribution<float> dis(-1.0f, 1.0f);

    vec3 v;
    do
    {
        v = vec3(dis(gen), dis(gen), 0.0f);
    } while (dot(v, v) >= 1.0f);
    return v;
}

#endif