#include "utilities.h"
#include "hittable_list.h"
#include "sphere.h"
#include "color.h"

#include <iostream>

using namespace std;

//
// ─────────────────────────────────────────────────────────────── UTILIDADES ─────
//

color ray_color(const Ray& ray, const hittable& world) {
    hit_record rec;

    if (world.hit(ray, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
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

    //
    // ──────────────────────────────────────────────────────────────────── MUNDO ─────
    //

    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    //
    // ─────────────────────────────────────────────────────────────────── CAMARA ─────
    //

    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * aspect_ratio;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    //
    // ─────────────────────────────────────────────────────────────────── RENDER ─────
    //

    cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        cerr << "\rLíneas restantes: " << j << ' ' << flush;

        for (int i = 0; i < image_width; ++i) {
            auto u = double(i)/(image_width-1);
            auto v = double(j)/(image_height-1);

            Ray r (origin, lower_left_corner + u * horizontal + v * vertical - origin);

            color pixel_color = ray_color(r, world);

            write_color(std::cout, pixel_color);
        }
    }

    cerr << "\nTerminado.\n";
}