#include <iostream>
#include <iomanip>
#include "../random.h"
#include "../utilities.h"

int main() {
    int N = 1000000;
    auto sum = 0.0;

    for (int i = 0; i < N; ++i) {
        auto r1 = random_double();
        auto r2 = random_double();

        auto x = cos(2*pi*r1) * 2 * sqrt(r2*(1 - r2));
        auto y = sin(2*pi*r1) * 2 * sqrt(r2*(1 - r2));
        auto z = 1 - r2;

        sum += z*z*z / (1.0/(2.0 * pi));
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Pi/2     = " << pi/2 << '\n';
    std::cout << "Estimacion = " << sum/N << '\n';

    return 0;
}