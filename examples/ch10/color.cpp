#include <cstdint>
#include <format>
#include <iostream>

// Упаковать три канала в одно число 0x00RRGGBB.
constexpr std::uint32_t rgb(std::uint32_t r, std::uint32_t g,
                            std::uint32_t b) {
    return (r << 16) | (g << 8) | b;
}

int main() {
    const std::uint32_t fire = rgb(255, 120, 40);   // огонь
    std::cout << std::format("fire   = 0x{:08X}\n", fire);

    // Распаковка: сдвигаем нужный байт вниз и отрезаем маской.
    const std::uint32_t r = (fire >> 16) & 0xFF;
    const std::uint32_t g = (fire >> 8) & 0xFF;
    const std::uint32_t b = fire & 0xFF;
    std::cout << std::format("r={} g={} b={}\n", r, g, b);

    // Затемнить вдвое: каждый канал пополам.
    const std::uint32_t dark = rgb(r / 2, g / 2, b / 2);
    std::cout << std::format("dark   = 0x{:08X}\n", dark);
    const std::uint32_t white = rgb(255, 255, 255);
    std::cout << std::format("white  = 0x{:08X}\n", white);
}
