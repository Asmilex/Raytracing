#ifndef UTILITIES_H
#define UTILITIES_H

#include <cmath>
#include <limits>
#include <memory>

//
// ────────────────────────────────────────────────────────── HEADERS COMUNES ─────
//

#include "ray.h"
#include "vec3.h"


//
// ─────────────────────────────────────────────────────────────────── USINGS ─────
//

using std::shared_ptr;
using std::make_shared;
using std::sqrt;


//
// ──────────────────────────────────────────────────────────────── CONSTANES ─────
//

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;


//
// ──────────────────────────────────────────────────────────────── FUNCIONES ─────
//

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180;
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

#endif