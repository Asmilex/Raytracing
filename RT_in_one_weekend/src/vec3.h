#ifndef VEC3_H
#define VEC3_H

#include "random.h"
#include <cmath>
#include <iostream>

using std::sqrt;

//
// ──────────────────────────────────────────────────────────────── CABECERAS ─────
//

class vec3;
using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream& os, const vec3& v);
inline vec3 operator+(const vec3& v1, const vec3& v2);
inline vec3 operator-(const vec3& v1, const vec3& v2);
inline vec3 operator*(const vec3& v1, const vec3& v2);
inline vec3 operator*(const vec3& v1, const double d);
inline vec3 operator*(const double d, const vec3& v1);
inline vec3 operator/(const vec3& v1, const double d);
inline double dot(const vec3& v1, const vec3& v2);
inline vec3 cross(const vec3& v1, const vec3& v2);

//
// ────────────────────────────────────────────────────────── CLASE PRINCIPAL ─────
//



class vec3 {
    public:
        vec3(): e{0, 0, 0} {}
        vec3(double x, double y, double z): e{x, y, z} {}

        inline double x() const {  return e[0]; }
        inline double y() const {  return e[1]; }
        inline double z() const {  return e[2]; }

        vec3 operator-() const {
            return vec3(-e[0], -e[1], -e[2]);
        }

        double  operator[] (int i) const { return e[i]; }
        double& operator[] (int i)       { return e[i]; }

        vec3& operator+=(const vec3& v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3& operator*= (const double d) {
            e[0] *= d;
            e[1] *= d;
            e[2] *= d;
            return *this;
        }

        vec3& operator/=(const double d) {
            return *this *= 1.0 / d;
        }


        double length() const {
            return sqrt(length_squared());
        }

        double length_squared() const {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }

        vec3 normalize() const {
            return *this/length();
        }

        inline static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        inline static vec3 random(double min, double max) {
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
        }

        bool near_zero() const {
            const auto epsilon = 1e-10;

            return (fabs(e[0]) < epsilon && fabs(e[1]) < epsilon && fabs(e[2]) < epsilon);
        }

    public:
        double e[3];
};

//
// ─────────────────────────────────────────────────────────────── UTILIDADES ─────
//

inline std::ostream& operator<<(std::ostream& os, const vec3& v) {
    return os << v.x() << " " << v.y() << " " << v.z();
}

inline vec3 operator+(const vec3& v1, const vec3& v2) {
    return vec3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

inline vec3 operator-(const vec3& v1, const vec3& v2) {
    return vec3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

inline vec3 operator*(const vec3& v1, const vec3& v2) {
    return vec3(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

inline vec3 operator*(const vec3& v1, const double d) {
    return vec3(v1.x() * d, v1.y() * d, v1.z() * d);
}

inline vec3 operator*(const double d, const vec3& v1) {
    return v1 * d;
}

inline vec3 operator/(const vec3& v1, const double d) {
    return v1 * (1.0 / d);
}

inline double dot(const vec3& v1, const vec3& v2) {
    return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

inline vec3 cross(const vec3& v1, const vec3& v2) {
    return vec3(v1.y() * v2.z() - v1.z() * v2.y(),
                v1.z() * v2.x() - v1.x() * v2.z(),
                v1.x() * v2.y() - v1.y() * v2.x()
            );
}

vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1, 1);

        if (p.length_squared() >= 1)
            continue;

        return p;
    }
}

vec3 random_unit_vector() {
    return random_in_unit_sphere().normalize();
}

vec3 random_in_hemisphere (const vec3& normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();

    if (dot(in_unit_sphere, normal) > 0.0) {
        return in_unit_sphere;
    }
    else {
        return -in_unit_sphere;
    }
}

vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

vec3 reflect(const vec3& v, const vec3& normal) {
    return v - 2.0 * dot(v, normal) * normal;
}

vec3 refract(const vec3& uv, const vec3& normal, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, normal), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * normal);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * normal;
    return r_out_parallel + r_out_perp;
}
#endif