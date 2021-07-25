#include "utilities.h"
#include "hittable_list.h"
#include "sphere.h"
#include "color.h"
#include "camera.h"
#include "random.h"
#include "material.h"
#include "moving_sphere.h"
#include "aarect.h"

#include <iostream>

using namespace std;

//
// ─────────────────────────────────────────────────────────────── UTILIDADES ─────
//

color ray_color(const Ray& r, const color& background, const hittable& world, int depth) {
    hit_record rec;

    // Si sobrepasamos el nivel máximo de profundidad, dejamos de acumular luz
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    // Si el rayo no toca nada, devolver el color del fondo
    if (!world.hit(r, 0.001, infinity, rec)) {
        return background;
    }

    Ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emmitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
        return emitted;
    }

    return emitted + attenuation * ray_color(scattered, background, world, depth-1);
}


//
// ────────────────────────────────────────────────────── I ──────────
//   :::::: E S C E N A S : :  :   :    :     :        :          :
// ────────────────────────────────────────────────────────────────
//


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


hittable_list simple_light() {
    hittable_list world;

    auto perlin_texture = make_shared<noise_texture>(16);
    world.add(
        make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(perlin_texture))
    );
    world.add(
        make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(perlin_texture))
    );

    auto panel_light = make_shared<diffuse_light>(10*color(0.4000, 0.4824, 0.4118));
    world.add(
        make_shared<xy_rect>(3, 5, 1, 3, -2, panel_light)
    );


    auto ball_light = make_shared<diffuse_light>(2*color(0.0314, 0.1216, 0.3176));
    world.add(
        make_shared<sphere>(point3(1, 4, 4), 1, ball_light)
    );

    return world;
}


hittable_list cornerll_box() {
    hittable_list objects;

    auto red = make_shared<lambertian>(color(.65, .05, .05));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    return objects;
}


//
// ──────────────────────────────────────────────── I ──────────
//   :::::: M A I N : :  :   :    :     :        :          :
// ──────────────────────────────────────────────────────────
//


int main() {
    auto aspect_ratio = 16.0/9.0;
    int samples_per_pixel = 100;

    int image_width = 800;

    hittable_list world;
    point3 lookfrom;
    point3 lookat;
    point3 vup = point3(0,1,0);
    double dist_to_focus = 10;
    double aperture;
    double fovy = 20.0;
    color background(0, 0, 0);

    const int scene = 6;

    switch (scene) {
        case 0:
            world = random_scene();

            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            aperture = 0.1;
            background = color(0.7, 0.8, 1.0);

            break;

        case 1:
            world = my_scene();

            lookfrom = point3(-2, 2, 1);
            lookat = point3(0, 0, -1);
            aperture = 0.1;
            background = color(0.7, 0.8, 1.0);

            break;

        case 2:
            world = two_spheres();

            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            fovy = 20.0;
            aperture = 0.0;
            background = color(0.7, 0.8, 1.0);

            break;

        case 3:
            world = two_perlin_spheres();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            fovy = 20.0;
            background = color(0.7, 0.8, 1.0);

            break;

        case 4:
            world = earth();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            fovy = 20.0;
            background = color(0.7, 0.8, 1.0);

            break;

        case 5:
            world = simple_light();
            samples_per_pixel = 400;
            background = color(0, 0, 0);
            lookfrom = point3(26,3,6);
            lookat = point3(0,2,0);
            fovy = 20.0;

            break;

        case 6:
            world = cornerll_box();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 200;
            background = color(0,0,0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            fovy = 40;

            break;

        default:
            background = color(0.7, 0.0, 1.0);
            break;
    }


    Camera camara(lookfrom, lookat, vup, fovy, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    int image_height = static_cast<int>(image_width/aspect_ratio);
    const int max_depth = 50;


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

                pixel_color += ray_color(r, background, world, max_depth);
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    cerr << "\nTerminado.\n";
}