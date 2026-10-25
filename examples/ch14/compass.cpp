#include <cmath>
#include <format>
#include <iostream>
#include <numbers>
#include <string_view>

// Ось y на карте смотрит вниз, поэтому угол растёт по часовой.
struct Dir {
    double degrees;
    std::string_view name;
};

int main() {
    constexpr Dir dirs[] = {{0, "east"}, {45, "south-east"},
                            {90, "south"}, {180, "west"},
                            {270, "north"}};
    for (const Dir& d : dirs) {
        const double a = d.degrees * std::numbers::pi / 180.0;
        std::cout << std::format("{:3} deg  cos {:5.2f}  sin {:5.2f}"
                                 "  {}\n",
                                 d.degrees, std::cos(a), std::sin(a),
                                 d.name);
    }
}
