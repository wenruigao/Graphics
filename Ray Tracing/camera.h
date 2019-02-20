#ifndef CAMERA_H
#define CAMERA_H

namespace rt
{
class camera
{
public:
  camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist)
  {
    float theta = vfov * M_PI / 180;
    float half_height = tan(theta / 2);
    float half_width = aspect * half_height;
    vec3 w = unit_vector(lookfrom - lookat);
    this->u = unit_vector(cross(vup, w));
    this->v = cross(w, this->u);
    this->origin = lookfrom;
    this->lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
    this->horizontal = 2.0f * half_width * focus_dist * u;
    this->vertical = 2.0f * half_height * focus_dist * v;
    this->lens_radius = aperture / 2.0f;
  }

  ray get_ray(float s, float t)
  {
    vec3 rd = this->lens_radius * random_in_unit_disk();
    vec3 offset = this->u * rd.x() + this->v * rd.y();
    return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
  }

private:
  vec3 origin, lower_left_corner, horizontal, vertical;
  vec3 u, v;
  float lens_radius;
};
} // namespace rt

#endif