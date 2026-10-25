#include <cmath>
#include <format>
#include <iostream>
#include <numbers>

struct Vec {
    double x = 0.0;
    double y = 0.0;
};

// Скалярное произведение: насколько b идёт вдоль a.
double dot(Vec a, Vec b) { return a.x * b.x + a.y * b.y; }
// Косое произведение: насколько b отходит вбок от a.
double cross(Vec a, Vec b) { return a.x * b.y - a.y * b.x; }
double length(Vec a) { return std::hypot(a.x, a.y); }

int main() {
    constexpr double kDeg = std::numbers::pi / 180.0;
    const double angle = 30.0 * kDeg;           // градусы → радианы
    const Vec dir{std::cos(angle), std::sin(angle)};   // взгляд
    const Vec player{2.0, 3.0};
    const Vec imp{6.0, 5.0};
    const Vec to{imp.x - player.x, imp.y - player.y};  // к импу

    std::cout << std::format("dir     ({:.3f}, {:.3f}) len {:.3f}\n",
                             dir.x, dir.y, length(dir));
    std::cout << std::format("to imp  ({}, {}) len {:.3f}\n", to.x,
                             to.y, length(to));
    std::cout << std::format("along   {:.3f}\n", dot(to, dir));
    std::cout << std::format("off     {:.3f}\n", cross(dir, to));
    const double deg = std::atan2(to.y, to.x) / kDeg;
    std::cout << std::format("bearing {:.1f} degrees\n", deg);
}
