#include <iostream>

struct Loose {
    char glyph;   // 1 байт
    int health;   // 4 байта
    char team;    // 1 байт
};

struct Tight {
    int health;   // 4 байта
    char glyph;   // 1 байт
    char team;    // 1 байт
};

int main() {
    std::cout << "Loose: " << sizeof(Loose) << " bytes\n";
    std::cout << "Tight: " << sizeof(Tight) << " bytes\n";
}
