#include <memory>
#include <limits>
#include <random>
#include "lodepng.h"
#include "vec3.h"
#include "ray.h"
#include "hitpoint.h"
#include "sphere.h"
#include "lambertian.h"
#include "metal.h"
#include "camera.h"

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

    auto hitables = vector<shared_ptr<hitable>>{
        make_shared<sphere>(vec3(0.0f, 0.0f, -1.0f), 0.5f, make_shared<lambertian>(vec3(0.8f, 0.3f, 0.3f))),
        make_shared<sphere>(vec3(0.0f, -100.5f, -1.0f), 100.0f, make_shared<lambertian>(vec3(0.8f, 0.8f, 0.0f))),
        make_shared<sphere>(vec3(1.0f, 0.0f, -1.0f), 0.5f, make_shared<metal>(vec3(0.8f, 0.6f, 0.2f), 1.0f)),
        make_shared<sphere>(vec3(-1.0f, 0.0f, -1.0f), 0.5f, make_shared<metal>(vec3(0.8f, 0.8f, 0.8f), 0.3f))};
    auto world = make_shared<hitable_list>(hitables);

    camera cam(
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 1.0f, 0.0f),
        90.0f,
        1.0f * nx / ny);
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
    }

    lodepng::encode(argv[1], image.get(), nx, ny);

    return 0;
}