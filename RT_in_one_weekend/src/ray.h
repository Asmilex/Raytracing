#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
    public:
        Ray() {}
        Ray(const vec3& origin, const vec3& direction) {
            this->orig = origin;
            this->dir = direction;
        }

        point3 origin() const {
            return orig;
        }

        vec3 direction() const {
            return this->dir;
        }

        point3 at (double t) const {
            return orig + dir * t;
        }

    public:
        point3 orig;
        vec3 dir;
};

#endif