#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

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
            return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
        }

    public:
        double e[3];
};

using point3 = vec3;
using color = vec3;

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



#endif