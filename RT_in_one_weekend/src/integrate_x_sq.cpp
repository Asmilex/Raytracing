#include "random.h"

#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>

inline double pdf(double x) {
    return 0.5 * x;
}

int main() {
    int N = 1000000;

    auto sum = 0.0;

    for (int i = 0; i < N; i++) {
        auto x = sqrt(random_double(0, 4));

        /*  Quote:
                (...) La función de densidad es una medida de qué cantidad de muestreo estamos haciendo.
                Para que se equilibre según la función de densidad pdf() que estemos usando,
                debemos hacer el peso proporcional a 1/pdf()
        */

        sum += x*x / pdf(x);
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Integral de x^2 entre 0 y 2 = " << sum/N << '\n';
}
