#include <cstddef>
#include <iostream>
#include <string>

// Обводка в одну строку: точка рядом с # становится #.
void outline(std::string& row) {
    for (std::size_t i = 0; i < row.size(); ++i) {
        const bool left = i > 0 && row[i - 1] == '#';
        const bool right = i + 1 < row.size() && row[i + 1] == '#';
        if (row[i] == '.' && (left || right)) {
            row[i] = '#';
        }
    }
}

int main() {
    std::string row = "....##......";
    outline(row);
    std::cout << row << '\n';     // ждали ...####.....
}
