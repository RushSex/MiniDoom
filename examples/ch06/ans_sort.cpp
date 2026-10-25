#include <algorithm>
#include <iostream>
#include <string_view>
#include <vector>

struct Monster {
    std::string_view name;
    int health = 0;
};

int main() {
    std::vector<Monster> monsters{{"imp", 20}, {"zombie", 5},
                                  {"baron", 1000}, {"imp", 12}};
    std::ranges::sort(monsters, {}, &Monster::health);
    for (const Monster& m : monsters) {
        std::cout << m.name << ' ' << m.health << '\n';
    }
}
