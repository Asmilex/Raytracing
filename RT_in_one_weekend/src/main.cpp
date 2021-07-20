#include "utilities.h"
#include "hittable_list.h"
#include "sphere.h"
#include "color.h"
#include "camera.h"
#include "random.h"

#include <iostream>

using namespace std;

//
// ─────────────────────────────────────────────────────────────── UTILIDADES ─────
//

color ray_color(const Ray& ray, const hittable& world, int depth) {
    hit_record rec;

    // Si sobrepasamos el nivel máximo de profundidad, dejamos de acumular luz
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    if (world.hit(ray, 0.00001, infinity, rec)) {
        point3 target = rec.p + rec.normal + random_in_unit_sphere();   // Ver dibujo de la sección 8
        return 0.5 * ray_color(
            Ray(rec.p, target - rec.p), world, depth-1
        );
    }

    vec3 unit_direction = ray.direction().normalize();
    auto t = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - t) * color (1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}


//
// ───────────────────────────────────────────────────────────────────── MAIN ─────
//

int main() {
    //
    // ─────────────────────────────────────────────────────────────────── IMAGEN ─────
    //

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 40;

    //
    // ──────────────────────────────────────────────────────────────────── MUNDO ─────
    //

    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    //
    // ─────────────────────────────────────────────────────────────────── CAMARA ─────
    //

    Camera camara;

    //
    // ─────────────────────────────────────────────────────────────────── RENDER ─────
    //

    cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        cerr << "\rLíneas restantes: " << j << ' ' << flush;

        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);

            for (auto s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);

                Ray r = camara.get_ray(u, v);

                pixel_color += ray_color(r, world, max_depth);
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    cerr << "\nTerminado.\n";
}