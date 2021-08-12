#include "../utilities.h"
#include "../random.h"

#include <iostream>
#include <iomanip>

int main() {
    int N = 100000;

    auto sum = 0.0;

    for (int i = 0; i < N; i++) {
        auto v = random_cosine_direction();
        sum += v.z() * v.z() * v.z() / (v.z()/pi);
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Pi/2     = " << pi/2 << '\n';
    std::cout << "Estimacion = " << sum/N << '\n';

}