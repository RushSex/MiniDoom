#include <algorithm>
#include <format>
#include <iostream>
#include <optional>
#include <span>
#include <string>
#include <vector>

struct Monster {
    std::string name;
    int hp = 0;
    double x = 0.0;
};

// span — «окошко» в чужой массив: без копирования.
int totalHp(std::span<const Monster> ms) {
    int sum = 0;
    for (const Monster& m : ms) {
        sum += m.hp;
    }
    return sum;
}

// optional — «может, есть, а может, и нет».
std::optional<Monster> find(std::span<const Monster> ms,
                            std::string_view name) {
    const auto it = std::ranges::find(ms, name, &Monster::name);
    if (it == ms.end()) {
        return std::nullopt;
    }
    return *it;
}

int main() {
    std::vector<Monster> ms{{.name = "imp", .hp = 60, .x = 3.5},
                            {.name = "zombie", .hp = 20},
                            {.name = "baron", .hp = 1000}};
    std::ranges::sort(ms, {}, &Monster::hp);       // по здоровью
    for (const Monster& m : ms) {
        std::cout << std::format("{:<7}{:>5}\n", m.name, m.hp);
    }
    std::cout << std::format("total {}\n", totalHp(ms));
    std::erase_if(ms, [](const Monster& m) { return m.hp > 500; });
    const auto imp = find(ms, "imp");
    std::cout << std::format("imp at {}, baron {}\n",
                             imp ? imp->x : -1.0,
                             find(ms, "baron") ? "here" : "gone");
}
