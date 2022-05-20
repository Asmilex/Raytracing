#include "utilities.h"
#include "hittable_list.h"
#include "sphere.h"
#include "color.h"
#include "camera.h"
#include "random.h"
#include "material.h"
#include "moving_sphere.h"
#include "aarect.h"
#include "box.h"
#include "constant-medium.h"
#include "bvh.h"
#include "pdf.h"
#include "scenes.hpp"

#include <iostream>
#include <chrono>

using namespace std;

//
// ─────────────────────────────────────────────────────────────── UTILIDADES ─────
//

color ray_color(const Ray& r, const color& background, const hittable& world, shared_ptr<hittable_list> lights, int depth) {
    hit_record rec;

    // Si sobrepasamos el nivel máximo de profundidad, dejamos de acumular luz
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    // Si el rayo no toca nada, devolver el color del fondo
    if (!world.hit(r, 0.001, infinity, rec)) {
        return background;
    }

    scatter_record srec;

    // Comprobar qué parte se dispersa y qué parte se emite.
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, srec))
        return emitted;

    if (srec.is_specular)
        return srec.attenuation * ray_color(srec.specular_ray, background, world, lights, depth - 1);

    // En caso en el que se disperse, muestreamos las luces de la escena con su función de densidad correspondiente, así como la emisión anterior.
    auto light_ptr = make_shared<hittable_pdf>(lights, rec.p);
    mixture_pdf f_density(light_ptr, srec.pdf_ptr);

    Ray scattered = Ray(rec.p, f_density.generate(), r.time());
    auto pdf_value = f_density.value(scattered.direction());

    return emitted
        + srec.attenuation * rec.mat_ptr->scattering_pdf(r, rec, scattered)
                           * ray_color(scattered, background, world, lights, depth - 1) / pdf_value;
}


//
// ──────────────────────────────────────────────── I ──────────
//   :::::: M A I N : :  :   :    :     :        :          :
// ──────────────────────────────────────────────────────────
//


int main() {
    auto aspect_ratio = 16.0/9.0;

    int samples_per_pixel = 1000;
    int image_width       = 720;
    int max_depth         = 10;

    hittable_list world;
    point3 lookfrom;
    point3 lookat;
    point3 vup = point3(0,1,0);
    double dist_to_focus = 10;
    double aperture;
    double fovy = 20.0;
    color background(0, 0, 0);

    const int scene = 10;

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
            world = cornell_box();
            aspect_ratio = 1.0;
            background = color(0,0,0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            fovy = 40;
            aperture = 0.0;

            break;

        case 7:
            world = cornell_smoke();
            aspect_ratio = 1.0;
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            fovy = 40.0;

            break;

        case 8:
            world = final_scene();
            aspect_ratio = 1.0;
            background = color(0, 0, 0);
            lookfrom = point3(478, 278, -600);
            lookat = point3(278, 278, 0);
            fovy = 40.0;

            break;

        case 9:
            world = bvh_debug();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 100;
            background = color(0,0,0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            fovy = 40;

            break;

        case 10:
            world = cornell_box_reflective();
            aspect_ratio = 1.0;
            background = color(0,0,0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            fovy = 40;
            aperture = 0.0;

            break;


        default:
            background = color(0.7, 0.0, 1.0);
            break;
    }


    Camera camara(lookfrom, lookat, vup, fovy, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    int image_height = static_cast<int>(image_width/aspect_ratio);


    // FIXME capítulo 10.2 muy temporal
    auto lights = make_shared<hittable_list>();
    lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));
    lights->add(make_shared<sphere>(point3(190, 90, 190), 90, shared_ptr<material>()));


    //
    // ─────────────────────────────────────────────────────────────────── RENDER ─────
    //

    cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    for (int j = image_height - 1; j >= 0; --j) {
        cerr << "\rLineas restantes: " << j << ' ' << flush;

        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);

            for (auto s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);

                Ray r = camara.get_ray(u, v);

                pixel_color += ray_color(r, background, world, lights, max_depth);
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cerr << "\nTerminado.\n";
    cerr << "Elapsed " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << endl;
}