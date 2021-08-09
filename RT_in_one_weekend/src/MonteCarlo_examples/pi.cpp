#include "../random.h"

#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>



int main() {
    // Estimar el valor de Pi usando el método de Monte Carlo.

    int radius = 1;
    int radius_squared = radius * radius;

    int inside_circle = 0;
    int inside_circle_stratified = 0;

    int sqrt_N = 10000;

    for (int i = 0; i < sqrt_N; ++i) {
        for (int j = 0; j < sqrt_N; ++j) {
            auto x = random_double(-radius, radius);
            auto y = random_double(-radius, radius);

            if (x*x + y*y < radius_squared) {
                inside_circle++;
            }

            // FIXME únicamente válido para radio 1.
            x = 2 * ((i + random_double())/sqrt_N) - 1;
            y = 2 * ((j + random_double())/sqrt_N) - 1;

            if (x*x + y*y < radius_squared) {
                inside_circle_stratified++;
            }

        }
    }

    auto N = static_cast<double>(sqrt_N) * sqrt_N;

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Estimacion de Pi habitual = "            << 4 * double(inside_circle) / N            << std::endl;
    std::cout << "Estimacion de Pi con estratificacion = " << 4 * double(inside_circle_stratified) / N << std::endl;

}