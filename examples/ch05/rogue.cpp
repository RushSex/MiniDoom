// rogue.cpp — консольный рогалик: доберись до выхода X
#include <array>
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>

constexpr int kHeight = 5;
constexpr std::array<std::string_view, kHeight> kLevel{
    "##########",
    "#@..#...X#",
    "#.#.#.##.#",
    "#.#...#..#",
    "##########",
};

// Клетка карты по координатам. Индексы у контейнеров беззнаковые,
// поэтому int переводим в std::size_t явно — и только здесь.
char cellAt(int x, int y) {
    const auto row = static_cast<std::size_t>(y);
    const auto col = static_cast<std::size_t>(x);
    return kLevel[row][col];
}

void drawMap(int px, int py) {
    for (int y = 0; y < kHeight; ++y) {
        std::string row{kLevel[static_cast<std::size_t>(y)]};
        if (y == py) {
            row[static_cast<std::size_t>(px)] = '@';
        }
        std::cout << row << '\n';
    }
}

int main() {
    int px = 1;                     // стартовая клетка: там, где @
    int py = 1;
    int moves = 0;
    drawMap(px, py);

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            break;                  // ввод кончился — выходим
        }
        bool bumped = false;
        for (char key : line) {     // каждая буква — шаг
            int dx = 0;
            int dy = 0;
            switch (key) {
            case 'w': dy = -1; break;
            case 's': dy = 1;  break;
            case 'a': dx = -1; break;
            case 'd': dx = 1;  break;
            default:  continue;     // прочие символы пропускаем
            }
            if (cellAt(px + dx, py + dy) == '#') {
                bumped = true;      // в стену не лезем
                continue;
            }
            px += dx;
            py += dy;
            ++moves;
            if (cellAt(px, py) == 'X') {
                std::cout << "EXIT! You escaped in " << moves
                          << " moves.\n";
                return 0;
            }
        }
        std::cout << (bumped ? "Bump! " : "") << "You are at (" << px
                  << ',' << py << ").\n";
    }
}
