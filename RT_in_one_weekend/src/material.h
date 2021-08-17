#ifndef MATERIAL_H
#define MATERIAL_H

#include "utilities.h"
#include "hittable.h"
#include "texture.h"
#include "onb.h"
#include "pdf.h"

struct hit_record;

struct scatter_record {
    Ray specular_ray;
    bool is_specular;
    color attenuation;
    shared_ptr<pdf> pdf_ptr;
};

// ────────────────────────────────────────────────────────────────────────────────

class material {
    public:
        virtual bool scatter(const Ray& r_in, const hit_record& rec, scatter_record& srec) const {
            return false;
        }

        virtual double scattering_pdf(const Ray& r_in, const hit_record& rec, const Ray& scattered) const {
            return 0;
        }

        virtual color emmitted(const Ray& r_in, const hit_record& rec, double u, double v, const point3& p) const {
            return color(0, 0, 0);
        }
};


// ────────────────────────────────────────────────────────────────────────────────


class lambertian : public material {
    public:
        lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
        lambertian(shared_ptr<texture> a) : albedo(a) {}

        virtual bool scatter(const Ray& r_in, const hit_record& rec, scatter_record& srec) const override {
            srec.is_specular = false;
            srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
            srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);     // NOTE ¿implementación del libro equivocada?

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

        virtual bool scatter(const Ray& r_in, const hit_record& rec, scatter_record& srec) const override {
            vec3 reflected = reflect(r_in.direction().normalize(), rec.normal);

            srec.is_specular = true;
            srec.specular_ray = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
            srec.attenuation = albedo;
            srec.pdf_ptr = 0;

            return true;
        }

    public:
        color albedo;
        double fuzz;     // Fuzziness. Ver 9.6.
};


// ────────────────────────────────────────────────────────────────────────────────


class dielectric : public material {
    public:
        dielectric(double index) : ir(index) {}

        virtual bool scatter(const Ray& r_in, const hit_record& rec, scatter_record& srec) const override {
            srec.is_specular = true;
            srec.pdf_ptr = nullptr;
            srec.attenuation = color(1.0, 1.0, 1.0);

            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;
            vec3 unit_direction = r_in.direction().normalize();

            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, refraction_ratio);

            srec.specular_ray = Ray(rec.p, direction, r_in.time());

            return true;
        }

    public:
        double ir; // Refractive index.

    private:
        static double reflectance(double cosine, double ref_idx) {
            // Usamos la aproximación de Schilck.
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;

            return r0 + (1 - r0) * pow(1 - cosine, 5);
        }
};


// ────────────────────────────────────────────────────────────────────────────────


class diffuse_light : public material {
    public:
        diffuse_light(shared_ptr<texture> a) : emit(a) {}
        diffuse_light(const color& a) : emit(make_shared<solid_color>(a)) {}

        virtual bool scatter(const Ray& r_in, const hit_record& rec, scatter_record& srec) const override {
            return false;
        }

        virtual color emmitted(const Ray& r_in, const hit_record& rec, double u, double v, const point3& p) const override {
            if (rec.front_face)
                return emit->value(u, v, p);
            else
                return color(0, 0, 0);
        }

    public:
        shared_ptr<texture> emit;
};


// ────────────────────────────────────────────────────────────────────────────────


class isotropic : public material {
    public:
        isotropic(color c) : albedo(make_shared<solid_color>(c)) {}
        isotropic(shared_ptr<texture> c) : albedo(c) {}

        virtual bool scatter(const Ray& r_in, const hit_record& rec, scatter_record& srec) const override {
            srec.is_specular = true;
            srec.specular_ray = Ray(rec.p, random_in_unit_sphere(), r_in.time());
            srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
            srec.pdf_ptr = 0;       // FIXME me lo he cuajao.

            return true;
        }

    public:
        shared_ptr<texture> albedo;
};


#endif