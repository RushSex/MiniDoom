#include <cmath>
#include <format>
#include <iostream>
#include <string_view>

struct Target {
    std::string_view name;
    double x;
    double y;
};

int main() {
    // Стреляем из (0, 0) строго на восток; стена на расстоянии 7.
    const double dx = 1.0, dy = 0.0;
    const double wall = 7.0;
    const double radius = 0.3;          // «толщина» монстра
    const Target targets[] = {{"imp", 5.0, 0.2},
                              {"zombie", 3.0, 0.5},
                              {"behind", 8.0, -0.1},
                              {"back", -2.0, 0.0}};
    for (const Target& t : targets) {
        const double along = t.x * dx + t.y * dy;          // вдоль
        const double off = std::abs(t.x * dy - t.y * dx);  // поперёк
        const bool hit = along > 0.0 && along < wall && off < radius;
        std::cout << std::format("{:<7} along {:5.1f}  off {:.1f}  "
                                 "{}\n",
                                 t.name, along, off,
                                 hit ? "HIT" : "miss");
    }
}
