#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "utilities.h"
#include "hittable.h"
#include "material.h"
#include "texture.h"

class constant_medium : public hittable {
    public:
        constant_medium (shared_ptr<hittable> b, double d, shared_ptr<texture> a)
            : boundary(b), neg_inv_density(-1/d), phase_function(make_shared<isotropic>(a)) {}

        constant_medium (shared_ptr<hittable> b, double d, color c)
            : boundary(b), neg_inv_density(-1/d), phase_function(make_shared<isotropic>(c)) {}


        virtual bool hit (const Ray& r, double t_min, double t_max, hit_record& rec) const override;


        virtual bool bounding_box (double time0, double time1, aabb& output_box) const override {
            return boundary->bounding_box(time0, time1, output_box);
        }

    public:
        shared_ptr<hittable> boundary;
        shared_ptr<material> phase_function;
        double neg_inv_density;
};


bool constant_medium::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
    // Mensajes de debuggeo útiles de vez en cuando.
    const bool enable_debug = true;
    const bool debugging = enable_debug && random_double() < 0.00001;

    hit_record rec1, rec2;

    if (!boundary->hit(r, -infinity, infinity, rec1))
        return false;

    if (!boundary->hit(r, rec1.t + 0.0000001, infinity, rec2))
        return false;


    if (rec1.t < t_min) rec1.t = t_min;
    if (rec2.t > t_max) rec2.t = t_max;


    if (rec1.t >= rec2.t)
        return false;

    if (rec1.t < 0)   {   rec1.t = 0;  }

    const auto ray_length = r.direction().length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance = neg_inv_density * log(random_double());

    if (debugging) {
        std::cerr << "hit distance = " << hit_distance << ", distance inside boundary = " << distance_inside_boundary << "\n";
    }

    if (hit_distance > distance_inside_boundary) {
        return false;
    }

    rec.t = rec1.t + hit_distance/ray_length;
    rec.p = r.at(rec.t);

    if (debugging) {
        std::cerr << "hit_distance = " << hit_distance << "\n"
                    << "rec.t = " << rec.t << "\n"
                    << "rec.p = " << rec.p << "\n";
    }

    rec.normal = vec3(1, 0, 0);     // Arbitrario.
    rec.front_face = true;          // También arbitrario.
    rec.mat_ptr = phase_function;

    return true;
}


#endif