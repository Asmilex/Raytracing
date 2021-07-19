#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

void write_color (std::ostream &os, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divir los colores por el número de muestras
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    os << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
       << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
       << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif