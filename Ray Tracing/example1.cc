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

        if (unit_direction.x() > 0.5f)
        {
            return vec3(0.89f, 0.73f, 0.25f); //sunshine color
        }
        else
        {
            float t = 0.5f * (unit_direction.y() + 1.0f);
            return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f); //sky color
        }
    }
}

struct circle
{
    float x, y, r;
};

shared_ptr<scene> make_scene()
{
    vector<unsigned char> image;
    unsigned width, height;
    lodepng::decode(image, width, height, "logo.png");

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0f, 1.0f);

    vector<circle> circles;

    const float radius = 0.01f;
    while (circles.size() < 500)
    {
        circle c0{dis(gen), dis(gen), radius + dis(gen) * radius};

        if (!all_of(circles.begin(), circles.end(), [&c0](circle c) {
                return (c.x - c0.x) * (c.x - c0.x) + (c.y - c0.y) * (c.y - c0.y) > (c.r + c0.r) * (c.r + c0.r);
            }))
        {
            continue;
        }

        if (image[(static_cast<int>(c0.y * 1024.0f) * 1024 + static_cast<int>(c0.x * 1024.0f)) * 4 + 3] != 0)
        {
            circles.push_back(c0);
            printf("\radded %lld circles", circles.size());
        }
    }

    printf("\n");

    auto scene = make_shared<rt::scene>();

    scene->add(make_shared<object>(
        make_shared<sphere>(vec3(0.5f, 0.5f, -10.0f), 10.0f),
        make_shared<metal>(vec3(0.5f, 0.5f, 0.5f), 0.3f)));

    for (circle c : circles)
    {
        float color = dis(gen) * 0.5f + 0.5f;

        scene->add(make_shared<object>(
            make_shared<sphere>(vec3(c.x, c.y, c.r), c.r),
            make_shared<metal>(vec3(color, color, color), dis(gen))));
    }

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

    vec3 lookfrom(0.5f, -1.2f, 2.2f);
    vec3 lookto(0.5f, 0.45f, 0.0f);
    float dist_to_focus = (lookfrom - lookto).length();
    float aperture = 0.1f;
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