#include <cmath>
#include <format>
#include <iostream>

// Стена толщиной в клетку: x от 5 до 6. Шар летит слева направо.
bool inWall(double x) {
    return x >= 5.0 && x < 6.0;
}

// Где шар остановился; substep — длина подшага или 0 (без них).
double fly(double speed, double dt, double substep) {
    double x = 0.0;
    while (x < 10.0) {                      // летим до x = 10
        const double travel = speed * dt;   // путь за кадр
        int steps = 1;
        if (substep > 0.0) {
            steps = static_cast<int>(std::ceil(travel / substep));
        }
        for (int i = 0; i < steps; ++i) {
            x += travel / steps;
            if (inWall(x)) {
                return x;                   // попал в стену
            }
        }
    }
    return x;                               // пролетел насквозь
}

int main() {
    const double dt = 1.0 / 30.0;           // игра тормозит: 30 FPS
    for (double speed : {6.0, 45.0}) {
        std::cout << std::format("speed {:4}: without {:5.2f}, "
                                 "with substeps {:5.2f}\n",
                                 speed, fly(speed, dt, 0.0),
                                 fly(speed, dt, 0.1));
    }
}
