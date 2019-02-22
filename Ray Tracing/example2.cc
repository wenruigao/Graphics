#include "rectangle.h"
#include "sphere.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "diffuse_light.h"
#include "constant_texture.h"
#include "image_texture.h"
#include "scene.h"
#include "camera.h"
#define LODEPNG_NO_COMPILE_CPP
#include "lodepng.h"
#include <memory>
#include <algorithm>

using namespace std;
using namespace rt;

static vec3 color(const ray &r, shared_ptr<scene> world, int depth = 0)
{
    hitpoint hp;
    shared_ptr<material> m;
    if (world->hit(r, 0.001f, numeric_limits<float>::max(), hp, m))
    {
        ray scattered;
        vec3 attenuation;
        vec3 emitted = m->emitted(hp.u(), hp.v(), hp.normal());
        if (depth < 50 && m->scatter(r, hp, attenuation, scattered))
        {
            return emitted + attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return emitted;
        }
    }
    else
    {
        return vec3(0.0f, 0.0f, 0.0f);
    }
}

struct circle
{
    float x, y, r;
};

shared_ptr<scene> make_scene()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0f, 1.0f);

    auto ct1 = make_shared<constant_texture>(vec3(0.8f, 0.8f, 0.8f));
    auto ct2 = make_shared<constant_texture>(vec3(0.5f, 0.5f, 0.5f));
    auto ct3 = make_shared<constant_texture>(vec3(0.89f, 0.74f, 0.25f));

    unsigned char *image;
    unsigned width, height;
    lodepng_decode32_file(&image, &width, &height, "earthmap.png");
    auto it = make_shared<image_texture>(image, width, height);

    auto m1 = make_shared<metal>(ct1, 0.05f);
    auto m2 = make_shared<metal>(ct1, 0.0f);
    auto d1 = make_shared<dielectric>(1.2f);
    auto l1 = make_shared<lambertian>(it);
    auto light = make_shared<diffuse_light>(ct3);
    auto p1 = vec3(0.0f, 0.0f, 0.0f);
    auto p2 = vec3(0.0f, 0.0f, 1.0f);
    auto p3 = vec3(1.0f, 0.0f, 0.0f);
    auto p4 = vec3(0.0f, 1.0f, 0.0f);
    auto p5 = vec3(1.0f, 0.0f, 1.0f);
    auto p6 = vec3(0.0f, 1.0f, 1.0f);
    auto p7 = vec3(1.0f, 1.0f, 0.0f);

    auto scene = make_shared<rt::scene>();
    scene->add(make_shared<object>(make_shared<rectangle>(p2, p1, p3), m1));
    scene->add(make_shared<object>(make_shared<rectangle>(p3, p1, p4), m1));
    scene->add(make_shared<object>(make_shared<rectangle>(p4, p1, p2), m1));
    scene->add(make_shared<object>(make_shared<rectangle>(p6, p2, p5), m1, d1));
    scene->add(make_shared<object>(make_shared<rectangle>(p5, p3, p7), m1, d1));
    scene->add(make_shared<object>(make_shared<rectangle>(p7, p4, p6), light, d1));

    scene->add(make_shared<object>(
        make_shared<sphere>(vec3(0.3f, 0.3f, 0.7f), 0.25f), l1));

    scene->add(make_shared<object>(
        make_shared<sphere>(vec3(0.7f, 0.3f, 0.3f), 0.25f), m2));

    scene->add(make_shared<object>(
        make_shared<rectangle>(vec3(5.0f, 0.0f, -95.0f), vec3(5.0f, 0.0f, 5.0f), vec3(-95.0f, 0.0f, 5.0f)),
        make_shared<metal>(ct2, 0.3f)));

    return scene;
}

int main(int argc, char *argv[])
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
    const unsigned int maxrgbvhex = 0x437fffff;
    const float maxrgbv = *reinterpret_cast<const float *>(&maxrgbvhex);
#pragma GCC diagnostic pop

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0f, 1.0f);

    const int nx = 1024;
    const int ny = 1024;
    const int ns = 100;
    auto image = make_unique<unsigned char[]>(nx * ny * 4);
    auto p = image.get();

    auto world = make_scene();

    vec3 lookfrom(3.6f, 1.8f, 4.8f);
    vec3 lookto(0.0f, 0.25f, -0.15f);
    vec3 center(0.5f, 0.5f, 0.5f);
    float dist_to_focus = (lookfrom - center).length();
    float aperture = 0.05f;

    camera cam(
        lookfrom, lookto,
        vec3(0.0f, 1.0f, 0.0f),
        20.0f,
        1.0f * nx / ny,
        aperture,
        dist_to_focus);

    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            vec3 col(0.0f, 0.0f, 0.0f);

            for (int s = 0; s < ns; s++)
            {
                float u = (static_cast<float>(i) + dis(gen)) / nx;
                float v = (static_cast<float>(j) + dis(gen)) / ny;

                col += color(cam.get_ray(u, v), world);
            }

            col /= static_cast<float>(ns);

            *(p++) = static_cast<unsigned char>(maxrgbv * sqrtf(col[0]));
            *(p++) = static_cast<unsigned char>(maxrgbv * sqrtf(col[1]));
            *(p++) = static_cast<unsigned char>(maxrgbv * sqrtf(col[2]));
            *(p++) = 255;
        }
        printf("\r%.0f%%", 100.0f * (ny - j - 1) / ny);
    }

    lodepng_encode32_file(argv[1], image.get(), nx, ny);

    return 0;
}