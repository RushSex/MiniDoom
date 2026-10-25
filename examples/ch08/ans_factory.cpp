#include <iostream>
#include <memory>
#include <string_view>

struct Monster {
    std::string_view name;
    int health = 0;
};

// Фабрика: создаёт монстра в куче и отдаёт владение вызывающему.
[[nodiscard]] std::unique_ptr<Monster> spawn(std::string_view name) {
    const int health = name == "imp" ? 60 : 20;
    return std::make_unique<Monster>(Monster{name, health});
}

int main() {
    auto imp = spawn("imp");
    auto zombie = spawn("zombie");
    std::cout << imp->name << ' ' << imp->health << ", "
              << zombie->name << ' ' << zombie->health << '\n';
}
