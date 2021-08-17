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

#include <iostream>

using namespace std;

//
// ─────────────────────────────────────────────────────────────── UTILIDADES ─────
//

color ray_color(const Ray& r, const color& background, const hittable& world, shared_ptr<hittable_list>& lights, int depth) {
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
    color emitted = rec.mat_ptr->emmitted(r, rec, rec.u, rec.v, rec.p);

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


hittable_list cornell_box() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<flip_face>(
            make_shared<xz_rect>(213, 343, 227, 332, 554, light)
        )
    );
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<material> aluminum = make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), aluminum);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(box1);

/*
    shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    objects.add(box2);
 */

    auto glass = make_shared<dielectric>(1.5);
    objects.add( make_shared<sphere>(point3(190, 90, 190), 90, glass) );

    return objects;
}


hittable_list cornell_smoke() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));

    shared_ptr<hittable> box2 = make_shared<box>(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));

    objects.add(make_shared<constant_medium>(box1, 0.01, color(0,0,0)));
    objects.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

    return objects;
}


hittable_list final_scene() {
    hittable_list boxes1;

    std::cerr << "Creating scene...\n";

    auto ground = make_shared<lambertian>(color(0.5333, 0.2000, 0.3569));

    const int boxes_per_side = 50;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
        }
    }

    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    auto moving_sphere_material = make_shared<lambertian>(color(0.1216, 0.4431, 0.5255));
    objects.add(
        make_shared<moving_sphere>(center1, center2, 0, 1, 40, moving_sphere_material)
    );

    objects.add(
        make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5))
    );
    objects.add(
        make_shared<sphere>(point3(0, 150, 145), 50, make_shared<metal>(color(0.3765, 0.2745, 0.2392), 1.0))
    );

    auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
    objects.add(boundary);
    objects.add(
        make_shared<constant_medium>(boundary, 0.2, color(0.9882, 0.9490, 0.6902))
    );

    // Fog
    boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    objects.add(
        make_shared<constant_medium>(boundary, .0001, color(1, 1, 1))
    );

    auto emat = make_shared<lambertian>(make_shared<image_texture>("RT_in_one_weekend/img/earthmap.jpg"));
    objects.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
    auto perlin_texture = make_shared<noise_texture>(0.1);
    objects.add(
        make_shared<sphere>(point3(220, 200, 300), 80, make_shared<lambertian>(perlin_texture))
    );

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.74, .74, .74));
    int ns = 1000;

    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
    }

    objects.add(
        make_shared<translate>(
            make_shared<rotate_y>(make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
            vec3(-100,270,395)
        )
    );


    std::cerr << "Scene loaded\n";
    return objects;
}


hittable_list default_final_scene() {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1,101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(point3(x0,y0,z0), point3(x1,y1,z1), ground));
        }
    }

    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30,0,0);
    auto moving_sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    objects.add(make_shared<sphere>(
        point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = make_shared<sphere>(point3(360,150,145), 70, make_shared<dielectric>(1.5));
    objects.add(boundary);
    auto pelota_azul = make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9));
    objects.add(pelota_azul);

/*     boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
    objects.add(make_shared<constant_medium>(boundary, .0001, color(1,1,1)));

 */    auto emat = make_shared<lambertian>(make_shared<image_texture>("RT_in_one_weekend/img/earthmap.jpg"));
    objects.add(make_shared<sphere>(point3(400,200,400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.1);
    objects.add(make_shared<sphere>(point3(220,280,300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0,165), 10, white));
    }

    objects.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
            vec3(-100,270,395)
        )
    );

    return objects;
}


hittable_list bvh_debug() {
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

    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    shared_ptr<hittable> nodos = make_shared<bvh_node>(box1, 0, 1);

    objects.add(nodos);

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
            world = cornell_box();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 100;
            background = color(0,0,0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            fovy = 40;
            aperture = 0.0;

            break;

        case 7:
            world = cornell_smoke();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 200;
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            fovy = 40.0;

            break;

        case 8:
            world = final_scene();
            aspect_ratio = 1.0;
            image_width = 800;
            samples_per_pixel = 10000;
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


        default:
            background = color(0.7, 0.0, 1.0);
            break;
    }


    Camera camara(lookfrom, lookat, vup, fovy, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    int image_height = static_cast<int>(image_width/aspect_ratio);
    const int max_depth = 50;

    // FIXME capítulo 10.2 muy temporal
    auto lights = make_shared<hittable_list>();
    lights->add(
        make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>())
    );
    lights->add(
        make_shared<sphere>(point3(190, 90, 190), 90, shared_ptr<material>())
    );

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

                pixel_color += ray_color(r, background, world, lights, max_depth);
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    cerr << "\nTerminado.\n";
}