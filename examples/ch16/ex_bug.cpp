#include <iostream>

int main() {
    const int w = 640;
    // Положение столбца на окне камеры: от −1 до +1.
    for (int col : {0, 200, 400, 639}) {
        const double cam = 2 * col / w - 1;
        std::cout << cam << ' ';
    }
    std::cout << '\n';
}
