#ifndef AABB_H
#define AABB_H

#include "utilities.h"

class aabb {
    public:
        aabb() {}
        aabb(const point3 &p1, const point3 &p2) {
            minimum = p1;
            maximum = p2;
        }

        point3 min() const { return minimum; }
        point3 max() const { return maximum; }

        /*
        Método original. Versión lenta.

        bool hit(const Ray& r, double t_min, double t_max) const {
            for (int a = 0; a < 3; a++) {
                auto t0 = fmin(
                    (minimum[a] - r.origin()[a])/ r.direction()[a],
                    (maximum[a] - r.origin()[a])/ r.direction()[a]
                );

                auto t1 = fmax(
                    (minimum[a] - r.origin()[a])/ r.direction()[a],
                    (maximum[a] - r.origin()[a])/ r.direction()[a]
                );

                t_min = fmax(t_min, t0);
                t_max = fmin(t_max, t1);

                if (t_max <= t_min) {
                    return false;
                }
            }

            return true;
        } */

        // Versión de Andrew Kensler. Más rápida.
        inline bool hit(const Ray& r, double t_min, double t_max) const {
            for (int a = 0;  a < 3; a++) {
                auto invD = 1.0f / r.direction()[a];
                auto t0 = (minimum[a] - r.origin()[a]) * invD;
                auto t1 = (maximum[a] - r.origin()[a]) * invD;

                if (invD < 0.0f) {
                    std::swap(t0, t1);
                }

                t_min = t0 > t_min ? t0 : t_min;
                t_max = t1 < t_max ? t1 : t_max;

                if (t_max <= t_min) {
                    return false;
                }
            }

            return true;
        }

    public:
        point3 minimum;
        point3 maximum;
};


aabb surrounding_box(aabb box0, aabb box1) {
    point3 small(
        fmin(box0.min().x(), box1.min().x()),
        fmin(box0.min().y(), box1.min().y()),
        fmin(box0.min().z(), box1.min().z())
    );

    point3 big(
        fmax(box0.min().x(), box1.min().x()),
        fmax(box0.min().y(), box1.min().y()),
        fmax(box0.min().z(), box1.min().z())
    );

    return aabb(small, big);
}

#endif