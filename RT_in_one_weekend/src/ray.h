#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
    public:
        Ray() {}
        Ray(const vec3& origin, const vec3& direction, double time = 0.0) : orig(origin), dir(direction), tm(time) {}

        point3 origin() const  { return orig; }
        vec3 direction() const { return this->dir; }
        double time() const    { return this->tm; }

        point3 at (double t) const {
            return orig + dir * t;
        }

    public:
        point3 orig;
        vec3 dir;
        double tm;
};

#endif