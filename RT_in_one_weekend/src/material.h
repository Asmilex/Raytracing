#ifndef MATERIAL_H
#define MATERIAL_H

#include "utilities.h"
#include "hittable.h"
#include "texture.h"
#include "onb.h"

struct hit_record;

class material {
    public:
        virtual bool scatter(const Ray& r_in, const hit_record& rec, color& albedo, Ray& scattered, double& pdf) const {
            return false;
        }

        virtual double scattering_pdf(const Ray& r_in, const hit_record& rec, const Ray& scattered) const {
            return 0;
        }

        virtual color emmitted(double u, double v, const point3& p) const {
            return color(0, 0, 0);
        }
};


// ────────────────────────────────────────────────────────────────────────────────


class lambertian : public material {
    public:
        lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
        lambertian(shared_ptr<texture> a) : albedo(a) {}

        virtual bool scatter(const Ray& r_in, const hit_record& rec, color& alb, Ray& scattered, double& pdf) const override {
            onb orthonormal_base;
            orthonormal_base.build_from_w(rec.normal);

            auto direction = orthonormal_base.local(random_cosine_direction());

            scattered = Ray(rec.p, direction.normalize(), r_in.time());
            alb = albedo->value(rec.u, rec.v, rec.p);
            pdf = dot(orthonormal_base.w(), scattered.direction()) / pi;

            return true;
        }

        double scattering_pdf(const Ray& r_in, const hit_record& rec, const Ray& scattered) const override {
            auto cosine = dot(rec.normal, scattered.direction().normalize());
            return cosine < 0 ? 0 : cosine/pi;
        }

    public:
        shared_ptr<texture> albedo;
};


// ────────────────────────────────────────────────────────────────────────────────


class metal : public material {
    public:
        metal(const color& a, double f) : albedo(a), fuzz(f < 1? f : 1) {}

        virtual bool scatter(const Ray& r_in, const hit_record& rec, color& alb, Ray& scattered, double& pdf) const override {
            vec3 reflected = reflect(r_in.direction().normalize(), rec.normal);

            scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
            alb = albedo;

            return dot(scattered.direction(), rec.normal) > 0;
        }


    public:
        color albedo;
        double fuzz;     // Fuzziness. Ver 9.6.
};


// ────────────────────────────────────────────────────────────────────────────────


class dielectric : public material {
    public:
        dielectric(double index) : ir(index) {}

        virtual bool scatter(const Ray& r_in, const hit_record& rec, color& albedo, Ray& scattered, double& pdf) const override {
            albedo = color(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;
            vec3 unit_direction = r_in.direction().normalize();

            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
                direction = reflect(unit_direction, rec.normal);
            }
            else {
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            }

            scattered = Ray(rec.p, direction, r_in.time());

            return true;
        }


    public:
        double ir; // Refractive index.

    private:
        static double reflectance(double cosine, double ref_idx) {
            // Usamos la aproximación de Schilck.
            auto r0 = (1- - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;

            return r0 + (1 - r0) * pow(1 - cosine, 5);
        }
};


// ────────────────────────────────────────────────────────────────────────────────


class diffuse_light : public material {
    public:
        diffuse_light(shared_ptr<texture> a) : emit(a) {}
        diffuse_light(const color& a) : emit(make_shared<solid_color>(a)) {}

        virtual bool scatter(const Ray& r_in, const hit_record& rec, color& albedo, Ray& scattered, double& pdf) const override {
            return false;
        }

        virtual color emmitted(double u, double v, const point3& p) const override {
            return emit->value(u, v, p);
        }

    public:
        shared_ptr<texture> emit;
};


// ────────────────────────────────────────────────────────────────────────────────


class isotropic : public material {
    public:
        isotropic(color c) : albedo(make_shared<solid_color>(c)) {}
        isotropic(shared_ptr<texture> c) : albedo(c) {}

        virtual bool scatter(const Ray& r_in, const hit_record& rec, color& alb, Ray& scattered, double& pdf) const override {
            scattered = Ray(rec.p, random_in_unit_sphere(), r_in.time());
            alb = albedo->value(rec.u, rec.v, rec.p);

            return true;
        }

    public:
        shared_ptr<texture> albedo;
};


#endif