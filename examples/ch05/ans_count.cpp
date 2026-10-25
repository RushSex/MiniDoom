#include <array>
#include <iostream>
#include <string_view>

constexpr std::array<std::string_view, 4> kLevel{
    "########",
    "#h..#.h#",
    "#..h...#",
    "########",
};

int main() {
    int medkits = 0;
    for (std::string_view row : kLevel) {
        for (char cell : row) {
            if (cell == 'h') {
                ++medkits;
            }
        }
    }
    std::cout << "medkits: " << medkits << '\n';
}
