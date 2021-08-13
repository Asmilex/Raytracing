#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "aabb.h"
#include "utilities.h"

class material;

struct hit_record {
    point3 p;
    vec3 normal;
    double t;
    bool front_face;
    shared_ptr<material> mat_ptr;
    double u;
    double v;

    inline void set_face_normal(const Ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ?
            outward_normal : -outward_normal;
    }
};


class hittable {
    public:
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};


class translate : public hittable {
    public:
        translate(shared_ptr<hittable> p, const vec3& displacement)
            : ptr(p), offset(displacement) {}

        virtual bool hit (const Ray& r, double t_min, double t_max, hit_record& rec) const override {
            Ray moved_r(r.origin() - offset, r.direction(), r.time());

            if (!ptr->hit(moved_r, t_min, t_max, rec)) {
                return false;
            }

            rec.p += offset;
            rec.set_face_normal(moved_r, rec.normal);

            return true;
        }

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            if (!ptr->bounding_box(time0, time1, output_box)) {
                return false;
            }

            output_box = aabb(
                output_box.min() + offset,
                output_box.max() + offset
            );

            return true;
        }

    public:
        shared_ptr<hittable> ptr;
        vec3 offset;
};


class rotate_y : public hittable {
    public:
        rotate_y (shared_ptr<hittable> p, double angle) : ptr(p) {
            auto radians = degrees_to_radians(angle);
            sin_theta = sin(radians);
            cos_theta = cos(radians);

            has_box = ptr->bounding_box(0, 1, bbox);

            point3 min(infinity, infinity, infinity);
            point3 max(-infinity, -infinity, -infinity);

            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 2; ++j) {
                    for (int k = 0; k < 2; ++k) {
                        auto x = i * bbox.max().x() + (1-i)*bbox.min().x();
                        auto y = j * bbox.max().y() + (1-j)*bbox.min().y();
                        auto z = k * bbox.max().z() + (1-k)*bbox.min().z();

                        auto x_new = cos_theta * x + sin_theta * z;
                        auto z_new = -sin_theta * x + cos_theta * z;

                        vec3 tester(x_new, y, z_new);

                        for (int c = 0; c < 3; ++c) {
                            min[c] = fmin(min[c], tester[c]);
                            max[c] = fmax(max[c], tester[c]);
                        }
                    }
                }
            }

            bbox = aabb(min, max);
        }

        virtual bool hit (const Ray& r, double t_min, double t_max, hit_record& rec) const override {
            auto origin = r.origin();
            auto direction = r.direction();

            origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
            origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

            direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
            direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

            Ray r_rotated (origin, direction, r.time());

            if (!ptr->hit(r_rotated, t_min, t_max, rec)) {
                return false;
            }

            auto p = rec.p;
            auto normal = rec.normal;

            p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
            p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

            normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
            normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

            rec.p = p;
            rec.set_face_normal(r_rotated, normal);

            return true;
        }

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            output_box = bbox;

            return true;
        }


    public:
        shared_ptr<hittable> ptr;
        double sin_theta, cos_theta;
        bool has_box;
        aabb bbox;
};


class flip_face : public hittable {
    public:
        flip_face(shared_ptr<hittable> p) : ptr(p) {}

        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override {
            if (!ptr->hit(r, t_min, t_max, rec)) {
                return false;
            }

            rec.front_face = !rec.front_face;
            return true;
        }

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            return ptr->bounding_box(time0, time1, output_box);
        }

    public:
        shared_ptr<hittable> ptr;
};

#endif