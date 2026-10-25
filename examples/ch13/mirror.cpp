#include <array>
#include <cstdint>
#include <iostream>

int main() {
    // Буква D: 7 строк, в каждой 5 точек. 1 — закрашено.
    const std::array<std::uint8_t, 7> letterD{
        0b11100,   // ###
        0b10010,   // #  #
        0b10001,   // #   #
        0b10001,   // #   #
        0b10001,   // #   #
        0b10010,   // #  #
        0b11100,   // ###
    };
    for (std::uint8_t row : letterD) {
        for (int col = 0; col < 5; ++col) {
            const int mask = 1 << col;        // 00001, 00010, …
            std::cout << ((row & mask) != 0 ? '#' : '.');
        }
        std::cout << "   0x" << std::hex << int{row} << std::dec
                  << '\n';
    }
}
