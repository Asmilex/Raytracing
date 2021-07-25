#include "utilities.h"
#include "hittable_list.h"
#include "sphere.h"
#include "color.h"
#include "camera.h"
#include "random.h"
#include "material.h"
#include "moving_sphere.h"

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
        /*  NOTE capítulo 8
            Podemos usar diferentes modelos de difusión para esta función.

            (1) El primero tiene mayor probailidad de que se difusen los rayos alrededor de la normal, y más baja en los extremos.
            La distribución es cos(phi^3), con phi el ángulo de la normal.
            Se debe usar la siguiente línea:
                point3 target = rec.p + rec.normal + random_in_unit_sphere();   // Ver dibujo de la sección 8

            (2) Para la distribución lambertiana, necesitamos cos(phi). Corregimos el anterior haciéndolo unitario:
                point3 target = rec.p + rec.normal + random_unit_vector();
            No es totalmente correcto. Para ello, se puede usar la siguiente versión.

            (3) La opción presentada en RT in one weekend no es correcta del todo (ver 8.6). Para hacerlo intuitivo,
            se puede hacer una difusión de los rayos uniforme, independientemente de la normal.
                point3 target = rec.p + random_in_hemisphere(rec.normal);

            Es recomendable variar entre uno y otro, para ver cómo quedan finalmente las escenas.

            Código del capítulo 8:
                point3 target = rec.p + rec.normal + random_in_unit_sphere();   // Ver dibujo de la sección 8
                return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, depth-1);
        */

        Ray scattered;
        color attenuation;

        if (rec.mat_ptr->scatter(ray, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth-1);
        }

        return color(0, 0, 0);
    }

    vec3 unit_direction = ray.direction().normalize();
    auto t = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - t) * color (1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}


hittable_list random_scene() {
    hittable_list world;

    auto ground_texture = make_shared<checkerboard>(color(0.4745, 0.5490, 0.6275), color(0.1765, 0.1765, 0.1765));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(ground_texture)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0,.5), 0);
                    world.add(make_shared<moving_sphere>(
                        center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}


hittable_list my_scene() {
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.66, 0.84, 0.39));
    auto material_center = make_shared<dielectric>(1.5);
    auto material_left   = make_shared<metal>(color(0.24, 0.39, 0.51), 0.3);
    auto material_right  = make_shared<metal>(color(0.51, 0.24, 0.39), 1.0);  // Color generado por Copilot, a ver qué da :)
    auto material_4      = make_shared<dielectric>(1.2);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100, material_ground));
    world.add(make_shared<sphere>(point3( 0.0, 0.0, -2.0), 0.6, material_center));
    world.add(make_shared<sphere>(point3(-1.2, 0.0, -1.0), 0.4, material_left));
    world.add(make_shared<sphere>(point3( 1.0, 0.0, -1.0), 0.5, material_right));
    world.add(make_shared<sphere>(point3(0.0, 1.0, -1.0), -0.3, material_4));

    return world;
}


hittable_list two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checkerboard>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return objects;
}


hittable_list two_perlin_spheres() {
    hittable_list objects;

    auto perlin_texture = make_shared<noise_texture>(8);
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(perlin_texture)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(perlin_texture)));

    return objects;
}


hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("RT_in_one_weekend/img/earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    return hittable_list(globe);
}

//
// ───────────────────────────────────────────────────────────────────── MAIN ─────
//

int main() {
    //
    // ─────────────────────────────────────────────────────────────────── IMAGEN ─────
    //

    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    //
    // ──────────────────────────────────────────────────────────────────── MUNDO ─────
    //


    hittable_list world;
    point3 lookfrom;
    point3 lookat;
    point3 vup = point3(0,1,0);
    double dist_to_focus = 10;
    double aperture;
    double fovy = 20.0;

    const int scene = 4;

    switch (scene) {
        case 0:
            world = random_scene();

            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            aperture = 0.1;

            break;

        case 1:
            world = my_scene();

            lookfrom = point3(-2, 2, 1);
            lookat = point3(0, 0, -1);
            aperture = 0.1;

            break;

        case 2:
            world = two_spheres();

            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            fovy = 20.0;
            aperture = 0.0;

            break;

        case 3:
            world = two_perlin_spheres();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            fovy = 20.0;
            break;

        default:
        case 4:
            world = earth();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            fovy = 20.0;
            break;
    }


    Camera camara(lookfrom, lookat, vup, fovy, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    //
    // ─────────────────────────────────────────────────────────────────── RENDER ─────
    //

    cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        cerr << "\rLineas restantes: " << j << ' ' << flush;

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