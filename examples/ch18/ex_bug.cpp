#include <cstdint>
#include <format>
#include <iostream>

std::uint32_t shade(std::uint32_t c, std::uint32_t s) {
    const std::uint32_t rb = ((c & 0xFF00FF) * s >> 8) & 0xFF00FF;
    const std::uint32_t g = ((c & 0x00FF00) * s >> 8) & 0x00FF00;
    return rb | g;
}

int main() {
    // Вспышка выстрела: делаем светлую стену в полтора раза ярче.
    const std::uint32_t wall = 0xC0C0C0;
    std::cout << std::format("{:06X}\n", shade(wall, 384));
}
