#include <iostream>
#include <string_view>

// Рисует полоску: name [#####-----] value/max
void printBar(std::string_view name, int value, int max) {
    const int width = 20;
    const int filled = value * width / max;
    std::cout << name << " [";
    for (int i = 0; i < width; ++i) {
        std::cout << (i < filled ? '#' : '-');
    }
    std::cout << "] " << value << '/' << max << '\n';
}

int main() {
    printBar("HEALTH", 65, 100);
    printBar("AMMO  ", 37, 200);
    printBar("ARMOR ", 0, 100);
}
