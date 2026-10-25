#include <cstdint>
#include <vector>

int main() {
    const int width = 640;
    const int height = 400;
    std::vector<std::uint32_t> pixels(width * height);

    // Рамка вокруг экрана: правый край — x = width... или нет?
    for (int y = 0; y < height; ++y) {
        const auto row = static_cast<std::size_t>(y * width);
        pixels[row] = 0xFFFFFF;                          // левый
        pixels[row + static_cast<std::size_t>(width)] = 0xFFFFFF;
    }
    return 0;
}
