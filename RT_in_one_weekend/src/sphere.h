#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "onb.h"
#include "pdf.h"

class sphere: public hittable {
    public:
        sphere() {}
        sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};

        virtual bool hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

        double pdf_value(const point3& o, const vec3& v) const {
            hit_record rec;

            if (!this->hit(Ray(o, v), 0.0001, infinity, rec))
                return 0;

            auto cos_theta_max = sqrt(1 - radius*radius/(center - o).length_squared());
            auto solid_angle = 2 * pi * (1 - cos_theta_max);

            return 1/solid_angle;
        }

        vec3 random(const point3& o) const {
            vec3 direction = center - o;
            auto distance_squared = direction.length_squared();
            onb base;
            base.build_from_w(direction);

            return base.local(random_to_sphere(radius, distance_squared));
        }

    public:
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;

    private:
        static void get_sphere_uv(const point3& p, double& u, double& v) {
            // p: punto de la esfera de radio 1, centrada en el origen
            // u: valor de retorno en [0, 1] del ángulo alrededor del eje y de x = -1
            // v: valor de retorno en [0, 1] del ángulo de y = -1 a y = 1.
            // Ejemplos:
            //   (1, 0, 0) |-> (0.5, 0.5)       (-1, 0, 0) |-> (0.0, 0.5)
            //   (0, 1, 0) |-> (0.5, 1.0)       (0, -1, 0) |-> (0.5, 0.0)
            //   (0, 0, 1) |-> (0.25, 0.5)      (0, 0, -1) |-> (0.75, 0.5)

            auto theta = acos(-p.y());
            auto phi = atan2(-p.z(), p.x()) + pi;

            u = phi/(2*pi);
            v = theta/(pi);
        }
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
    vec3 outward_normal = (rec.p - center)/radius;
    rec.set_face_normal(ray, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
}


bool sphere::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = aabb(
        center - vec3(radius, radius, radius),
        center + vec3(radius, radius, radius)
    );

    return true;
}


#endif