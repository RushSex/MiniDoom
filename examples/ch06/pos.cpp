#include <iostream>

struct Pos {
    int x = 0;
    int y = 0;
    bool operator==(const Pos&) const = default;  // по всем полям
};

int main() {
    Pos player{3, 1};
    Pos exit{8, 1};
    std::cout << std::boolalpha;
    std::cout << (player == exit) << '\n';
    player.x += 5;
    std::cout << (player == exit) << '\n';
}
