#include <memory>
#include <limits>
#include <random>
#include "lodepng.h"
#include "vec3.h"
#include "ray.h"
#include "hitpoint.h"
#include "camera.h"
#include "sphere.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

using namespace std;

static vec3 color(const ray &r, shared_ptr<hitable> world, int depth = 0)
{
    float t;
    hitpoint hp;
    if (world->hit(r, 0.001f, numeric_limits<float>::max(), t, hp))
    {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && hp.material->scatter(r, hp.point, hp.normal, attenuation, scattered))
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

shared_ptr<hitable> random_scene()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0f, 1.0f);

    vector<shared_ptr<hitable>> list;
    list.push_back(make_shared<sphere>(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, make_shared<lambertian>(vec3(0.5f, 0.5f, 0.5f))));
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float choose_mat = dis(gen);
            vec3 center(static_cast<float>(a) + 0.9f * dis(gen), 0.2f, static_cast<float>(b) + 0.9f * dis(gen));
            if ((center - vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f)
            {
                if (choose_mat < 0.8f)
                {
                    list.push_back(make_shared<sphere>(center, 0.2f, make_shared<lambertian>(vec3(dis(gen) * dis(gen), dis(gen) * dis(gen), dis(gen) * dis(gen)))));
                }
                else if (choose_mat < 0.95f)
                {
                    list.push_back(make_shared<sphere>(center, 0.2f, make_shared<metal>(vec3(0.5f * (1.0f + dis(gen)), 0.5f * (1.0f + dis(gen)), 0.5f * (1.0f + dis(gen))), 0.5f * dis(gen))));
                }
                else
                {
                    list.push_back(make_shared<sphere>(center, 0.2f, make_shared<dielectric>(1.5f)));
                }
            }
        }
    }

    list.push_back(make_shared<sphere>(vec3(0.0f, 1.0f, 0.0f), 1.0f, make_shared<dielectric>(1.5f)));
    list.push_back(make_shared<sphere>(vec3(-4.0f, 1.0f, 0.0f), 1.0f, make_shared<lambertian>(vec3(0.4f, 0.2f, 0.1f))));
    list.push_back(make_shared<sphere>(vec3(4.0f, 1.0f, 0.0f), 1.0f, make_shared<metal>(vec3(0.7f, 0.6f, 0.5f), 0.0f)));

    return make_shared<hitable_list>(list);
}

int main(int argc, char *argv[])
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0f, 1.0f);

    const int nx = 1920;
    const int ny = 1080;
    const int ns = 100;
    auto image = make_unique<unsigned char[]>(nx * ny * 4);
    auto p = image.get();

    auto world = random_scene();

    vec3 lookfrom(13.0f, 2.0f, 3.0f);
    vec3 lookto(0.0f, 0.0f, 0.0f);
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
        printf("\r%.0f", 100.0f * (ny - j - 1) / ny);
    }

    lodepng::encode(argv[1], image.get(), nx, ny);

    return 0;
}