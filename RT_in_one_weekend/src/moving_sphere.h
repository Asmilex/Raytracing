#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "utilities.h"
#include "random.h"
#include "hittable.h"

class moving_sphere : public hittable {
    public:
        moving_sphere() {}
        moving_sphere(point3 cen0, point3 cen1, double _time0, double _time1, double r, shared_ptr<material> m)
            : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m) {}


        point3 center(double time) const {
            return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
        }

        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual bool bounding_box(double _time0, double _time1, aabb& output_box) const override;


    public:
        point3 center0, center1;
        double time0, time1;
        double radius;
        shared_ptr<material> mat_ptr;
};


bool moving_sphere::hit(const Ray& ray, double t_min, double t_max, hit_record& rec) const {
    vec3 d = ray.origin() - center(ray.time());

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
    vec3 outward_normal = (rec.p - center(ray.time()))/radius;
    rec.set_face_normal(ray, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}


bool moving_sphere::bounding_box(double _time0, double _time1, aabb& output_box) const {
    aabb box0(
        center(_time0) - vec3(radius, radius, radius),
        center(_time0) + vec3(radius, radius, radius)
    );

    aabb box1(
        center(_time1) - vec3(radius, radius, radius),
        center(_time1) + vec3(radius, radius, radius)
    );

    output_box = surrounding_box(box0, box1);

    return true;
}

#endif
