#include <cmath>
#include <format>
#include <iostream>
#include <numbers>

// Четыре формы волны: значение в точке phase от 0 до 1.
double sine(double phase) {
    return std::sin(2.0 * std::numbers::pi * phase);
}
double saw(double phase) { return 2.0 * phase - 1.0; }
double square(double phase) { return phase < 0.5 ? 1.0 : -1.0; }

int main() {
    std::cout << "phase   sine    saw  square\n";
    for (int i = 0; i < 8; ++i) {
        const double p = i / 8.0;
        std::cout << std::format("{:5.3f} {:6.2f} {:6.2f} {:7.2f}\n",
                                 p, sine(p) + 0.0, saw(p),
                                 square(p));
    }
    // Сэмпл — это значение волны в момент времени. 440 Гц, ля:
    constexpr int kRate = 22050;
    std::cout << "A4 at 22050 Hz:";
    for (int i = 0; i < 6; ++i) {
        const double phase = 440.0 * i / kRate;   // сколько периодов
        const double s = sine(phase - std::floor(phase));
        std::cout << ' ' << static_cast<int>(s * 32767);
    }
    std::cout << '\n';
}
