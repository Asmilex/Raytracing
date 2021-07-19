#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere: public hittable {
    public:
        sphere() {}
        sphere(point3 cen, double r) : center(cen), radius(r) {};

        virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 center;
        double radius;
};

// Devuelve si un rayo alcanza a la esfera dentro del intervalo [t_min, t_max]. De ser así,
// la información se pasa en rec.
bool sphere::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const {
    vec3 d = ray.origin() - center;

    auto a = ray.direction().length_squared();
    auto half_b = dot(d, ray.direction());
    auto c = d.length_squared() - radius * radius;

    auto discriminant = half_b * half_b - a * c;

    if (discriminant < 0.0) {
        return false;
    }

    auto sqrtd = sqrt(discriminant);

    // Encontrar la raíz que se queda en el rango [t_min, t_max].
    auto root = (-half_b - sqrtd) / a;

    if (root < t_min || root > t_max) {
        root = (-half_b + sqrtd) / a;

        if (root < t_min || root > t_max) {
            return false;
        }
    }

    rec.t = root;
    rec.p = ray.at(rec.t);
    rec.normal = (rec.p - center) / radius;

    return true;
}

#endif