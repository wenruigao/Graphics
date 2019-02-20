#include "rectangle.h"
#include "sphere.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "scene.h"
#include "camera.h"
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
        if (depth < 50 && m->scatter(r, hp.point(), hp.normal(), attenuation, scattered))
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return vec3(0.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());

        float t = 0.5f * (unit_direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
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

    auto m1 = make_shared<metal>(vec3(0.8f, 0.8f, 0.8f), 0.05f);
    auto d1 = make_shared<dielectric>(1.2f);
    auto p1 = vec3(0.0f, 0.0f, 0.0f);
    auto p2 = vec3(1.0f, 0.0f, 0.0f);
    auto p3 = vec3(0.0f, 1.0f, 0.0f);
    auto p4 = vec3(0.0f, 0.0f, 1.0f);
    auto p5 = vec3(1.0f, 1.0f, 0.0f);
    auto p6 = vec3(1.0f, 0.0f, 1.0f);
    auto p7 = vec3(0.0f, 1.0f, 1.0f);

    auto scene = make_shared<rt::scene>();
    scene->add(make_shared<object>(make_shared<rectangle>(p2, p1, p3), m1));
    scene->add(make_shared<object>(make_shared<rectangle>(p3, p1, p4), m1));
    scene->add(make_shared<object>(make_shared<rectangle>(p4, p1, p2), m1));
    scene->add(make_shared<object>(make_shared<rectangle>(p6, p2, p5), m1, d1));
    scene->add(make_shared<object>(make_shared<rectangle>(p5, p3, p7), m1, d1));
    scene->add(make_shared<object>(make_shared<rectangle>(p6, p4, p7), d1));

    vector<circle> circles;
    while (circles.size() < 10)
    {
        circle c0{dis(gen) * 0.7f + 0.15f, dis(gen) * 0.7f + 0.15f, dis(gen) * 0.15f};

        if (!all_of(circles.begin(), circles.end(), [&c0](circle c) {
                return (c.x - c0.x) * (c.x - c0.x) + (c.y - c0.y) * (c.y - c0.y) > (c.r + c0.r) * (c.r + c0.r);
            }))
            continue;

        circles.push_back(c0);

        if (c0.r < 0.12f)
        {
            scene->add(make_shared<object>(
                make_shared<sphere>(vec3(c0.x, c0.y, c0.r), c0.r),
                make_shared<lambertian>(vec3(dis(gen), dis(gen), dis(gen)))));
        }
        else
        {
            scene->add(make_shared<object>(
                make_shared<sphere>(vec3(c0.x, c0.y, c0.r), c0.r),
                make_shared<metal>(vec3(dis(gen), dis(gen), dis(gen)), 0.0f)));
        }
    }

    scene->add(make_shared<object>(
        make_shared<rectangle>(vec3(-95.0f, 5.0f, 0.0f), vec3(5.0f, 5.0f, 0.0f), vec3(5.0f, -95.0f, 0.0f)),
        make_shared<metal>(vec3(0.5f, 0.5f, 0.5f), 0.3f)));

    return scene;
}

int main(int argc, char *argv[])
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0f, 1.0f);

    const int nx = 1024;
    const int ny = 1024;
    const int ns = 100;
    auto image = make_unique<unsigned char[]>(nx * ny * 4);
    auto p = image.get();

    auto world = make_scene();

    vec3 lookfrom(4.8f, 3.6f, 1.8f);
    vec3 lookto(-0.15f, 0.0f, 0.25f);
    float dist_to_focus = (lookfrom - lookto).length();
    float aperture = 0.05f;

    camera cam(
        lookfrom, lookto,
        vec3(0.0f, 0.0f, 1.0f),
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

            *(p++) = static_cast<unsigned char>(255.99f * sqrtf(col[0]));
            *(p++) = static_cast<unsigned char>(255.99f * sqrtf(col[1]));
            *(p++) = static_cast<unsigned char>(255.99f * sqrtf(col[2]));
            *(p++) = 255;
        }
        printf("\r%.0f%%", 100.0f * (ny - j - 1) / ny);
    }

    lodepng::encode(argv[1], image.get(), nx, ny);

    return 0;
}