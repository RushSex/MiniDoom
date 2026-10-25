#include <iostream>

int main() {
    const int h = 400;                  // высота экрана
    for (double dist : {0.5, 2.0, 3.0, 8.0}) {
        std::cout << static_cast<int>(h / dist) << ' ';
    }
    std::cout << '\n';
}
