#include <cstdint>
#include <iostream>
#include <vector>

// Микшер без защиты: складываем два громких звука.
int main() {
    const std::vector<float> shot{0.8f, 0.9f, -0.7f};
    const std::vector<float> boom{0.7f, 0.6f, -0.8f};
    for (std::size_t i = 0; i < shot.size(); ++i) {
        const float sum = shot[i] + boom[i];
        const auto s = static_cast<std::int16_t>(sum * 32767.0f);
        std::cout << s << ' ';
    }
    std::cout << '\n';
}
