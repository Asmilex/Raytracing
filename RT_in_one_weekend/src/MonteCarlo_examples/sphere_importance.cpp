#include "../utilities.h"
#include <iostream>
#include <iomanip>

inline double pdf(const vec3& p) {
    // Esfera unidad
    return 1/(4*pi);
}

int main() {
    int N = 100000;

    auto sum = 0.0;

    for (int i = 0; i < N; i++) {
        vec3 d = random_unit_vector();
        auto cosine_squared = d.z()*d.z();
        sum += cosine_squared/pdf(d);
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Integral de coseno^2(tita) en la ¿esfera/circunferencia? unidad = " << sum/N << '\n';

}