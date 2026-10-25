#include <array>
#include <iostream>
#include <string_view>

constexpr std::array<std::string_view, 5> kLevel{
    "##########",
    "#@..#...X#",
    "#.#.#.##.#",
    "#.#...#..#",
    "##########",
};

int main() {
    int walls = 0;
    int floor = 0;
    for (std::string_view row : kLevel) {      // строка за строкой
        std::cout << row << '\n';
        for (char cell : row) {                // клетка за клеткой
            if (cell == '#') {
                ++walls;
            } else {
                ++floor;
            }
        }
    }
    std::cout << "walls: " << walls << ", floor: " << floor << '\n';
    std::cout << "cell (8,1): " << kLevel[1][8] << '\n';
}
