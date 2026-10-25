#include <cmath>
#include <iostream>

// Приводим угол в градусах к диапазону от -180 до 180.
double wrap(double deg) {
    return std::remainder(deg, 360.0);
}

int main() {
    std::cout << wrap(370) << ' ' << wrap(190) << ' ' << wrap(-200)
              << '\n';
}
