#ifndef AARECT_H
#define AARECT_H

#include "utilities.h"
#include "hittable.h"

class xy_rect : public hittable {
    public:
        xy_rect() {}
        xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<material> material)
            : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(material) {}

        virtual bool hit (const Ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box (double time0, double time1, aabb& output_box) const override {
            // La bounding box tiene que tener ancho > 0 en cada dimensión, así que le metemos a z una pequeña cantidad.
            output_box = aabb(
                point3(x0, y0, k - 0.00001),
                point3(x1, y1, k + 0.00001)
            );

            return true;
        }

    public:
        shared_ptr<material> mp;
        double x0, x1, y0, y1, k;
};


bool xy_rect::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
    auto t = (k - r.origin().z()) / r.direction().z();

    if (t < t_min || t > t_max) {
        return false;
    }

    auto x = r.origin().x() + t * r.direction().x();
    auto y = r.origin().y() + t * r.direction().y();

    if (x < x0 || x > x1 || y < y0 || y > y1) {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}

#endif