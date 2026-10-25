#include <cstdint>
#include <format>
#include <iostream>

int main() {
    const std::uint32_t c = 0xFF8001;       // R = FF, G = 80, B = 01
    const std::uint32_t a = c >> 1;
    const std::uint32_t b = (c >> 1) & 0x7F7F7F;
    std::cout << std::format("{:06X} {:06X}\n", a, b);
}
