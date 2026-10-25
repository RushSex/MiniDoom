// main.cpp — три вида шума буквами: белый, гладкий и fbm
#include "noise.hpp"

#include <iostream>
#include <string_view>

int main() {
    constexpr std::string_view ramp = " .:-=+*#%@";  // темно→светло
    const auto letter = [&](double v) {               // v от 0 до 1
        return ramp[static_cast<std::size_t>(v * 9.99)];
    };
    std::cout << "hash01               valueNoise           fbm\n";
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 20; ++x) {      // каждая точка — своя
            std::cout << letter(doom::hash01(x, y, 7));
        }
        std::cout << ' ';
        for (int x = 0; x < 20; ++x) {      // 4 точки шума на 20
            std::cout << letter(doom::valueNoise(x / 5.0, y / 5.0,
                                                 64, 7));
        }
        std::cout << ' ';
        for (int x = 0; x < 20; ++x) {      // 4 октавы
            std::cout << letter(doom::fbm(x / 5.0, y / 5.0, 4, 64,
                                          7));
        }
        std::cout << '\n';
    }
}
