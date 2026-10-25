#include <format>
#include <iostream>
#include <vector>

int main() {
    const int width = 4;
    const int height = 3;
    std::vector<char> screen(width * height, '.');

    // Пиксель (x, y) лежит в векторе под номером y * width + x.
    auto at = [&](int x, int y) -> char& {
        return screen[static_cast<std::size_t>(y * width + x)];
    };
    at(0, 0) = 'A';
    at(3, 0) = 'B';
    at(1, 2) = 'C';

    for (char c : screen) {                 // как лежит в памяти
        std::cout << c;
    }
    std::cout << '\n';
    for (int y = 0; y < height; ++y) {      // как видим на экране
        for (int x = 0; x < width; ++x) {
            std::cout << at(x, y);
        }
        std::cout << '\n';
    }
    std::cout << std::format("C is at index {}\n", 2 * width + 1);
}
