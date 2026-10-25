#include <cmath>
#include <iostream>

int main() {
    const double dt = 1.0 / 60.0;
    for (double speed : {3.0, 6.6, 12.0}) {
        const double travel = speed * dt;
        std::cout << std::ceil(travel / 0.1) << ' ';
    }
    std::cout << '\n';
}
