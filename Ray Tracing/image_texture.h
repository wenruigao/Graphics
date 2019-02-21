#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H
#include "texture.h"
#include <memory>

using namespace std;

namespace rt
{
class image_texture : public texture
{
  public:
    image_texture(unsigned char * pixels, int width, int height)
        : pixels(pixels), width(width), height(height) {}

    virtual vec3 value(float u, float v, const vec3 &p) const
    {
        int i = u * this->width;
        int j = (1.0f - v) * this->height;
        i = max(0, i);
        j = max(0, j);
        i = min(width - 1, i);
        j = min(height - 1, j);
        auto *pt = &this->pixels[(j * this->width + i) * 4];
        float r = 1.0f * pt[0] / 255;
        float g = 1.0f * pt[1] / 255;
        float b = 1.0f * pt[2] / 255;
        float a = 1.0f * pt[3] / 255;
        r *= a;
        g *= a;
        b *= a;
        return vec3(r, g, b);
    }

  private:
    unique_ptr<unsigned char[]> pixels;
    int width, height;
};
} // namespace rt

#endif