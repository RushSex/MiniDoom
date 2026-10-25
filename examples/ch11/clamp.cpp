#include <algorithm>
#include <format>
#include <iostream>

int main() {
    const double speed = 120.0;              // пикселей в секунду
    const double frames[] = {0.016, 0.017, 2.5, 0.016};  // 2,5 с!
    double x = 0.0;
    double xClamped = 0.0;
    for (double dt : frames) {
        x += speed * dt;
        xClamped += speed * std::min(dt, 0.05);   // не больше 50 мс
        std::cout << std::format("dt {:5.3f}: x {:6.1f}, "
                                 "clamped {:5.1f}\n",
                                 dt, x, xClamped);
    }
}
