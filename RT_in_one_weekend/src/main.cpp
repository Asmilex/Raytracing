#include "color.h"
#include "vec3.h"
#include "ray.h"
#include <iostream>

using namespace std;

//
// ─────────────────────────────────────────────────────────────── UTILIDADES ─────
//

bool hit_sphere (const point3& center, double radius, const Ray& ray) {
    // Resolvemos ecuación de forma que, dado un cierto rayo P(t) con centro A, dirección b,
    // y una esfera de centro `center`, radio `radius`, hayamos si ha impactado.
    // Se trata de ver si (P(t) - A) = radius^2 <=> t^2 * b + 2tb * (A - center) + (A - center)^2 - radius^2 = 0
    vec3 d = ray.origin() - center;

    auto a = dot(ray.direction(), ray.direction());
    auto b = 2.0 * dot(d, ray.direction());
    auto c = dot(d, d) - radius * radius;

    auto discriminant = b * b - 4.0 * a * c;

    return discriminant >= 0.0;
}


color ray_color(const Ray& ray) {
    if (hit_sphere(point3(0, 0, -1), 0.5, ray)) {
        return color(1, 0, 0);
    }

    vec3 direction = ray.direction().normalize();
    auto t = 0.5 * (1.0 + direction.y());
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

//
// ───────────────────────────────────────────────────────────────────── MAIN ─────
//


int main() {
    //
    // ─────────────────────────────────────────────────────────────────── IMAGEN ─────
    //

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width/aspect_ratio);

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

            color pixel_color = ray_color(r);

            write_color(std::cout, pixel_color);
        }
    }

    cerr << "\nTerminado.\n";
}