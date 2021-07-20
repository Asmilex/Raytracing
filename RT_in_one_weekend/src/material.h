#ifndef MATERIAL_H
#define MATERIAL_H

#include "utilities.h"
#include "hittable.h"

struct hit_record;

class material {
    public:
        virtual bool scatter(const Ray& r_in, const hit_record& rec, color& attenuation, Ray& scattered) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const color& a) : albedo(a) {}

        virtual bool scatter(const Ray& r_in, const hit_record& rec, color& attenuation, Ray& scattered) const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            // Debemos tener cuidado con los scatters cerca de 0. Podrían producir infinitos y NaNs.
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }

            scattered = Ray(rec.p, scatter_direction);
            attenuation = albedo;

            return true;
        }

    public:
        color albedo;
};


class metal : public material {
    public:
        metal(const color& a, double f) : albedo(a), fuzz(f < 1? f : 1) {}

        virtual bool scatter(const Ray& r_in, const hit_record& rec, color& attenuation, Ray& scattered) const override {
            vec3 reflected = reflect(r_in.direction().normalize(), rec.normal);

            scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
            attenuation = albedo;

            return dot(scattered.direction(), rec.normal) > 0;
        }


    public:
        color albedo;
        double fuzz;     // Fuzziness. Ver 9.6.
};

#endif