#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// Шаблон функции: F — любой тип, который можно вызвать как f(x, y).
template <typename F>
void forEachCell(int w, int h, F f) {
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            f(x, y);
        }
    }
}

int main() {
    const int w = 36;
    const int h = 13;
    std::vector<std::string> pic(h, std::string(w, '.'));
    // Лямбда захватывает pic по ссылке и прячет приведения типов.
    const auto put = [&pic](int x, int y, char c) {
        const auto row = static_cast<std::size_t>(y);
        pic[row][static_cast<std::size_t>(x)] = c;
    };
    // Эллипс с центром (10, 6) и полуосями 9 и 6, «с объёмом».
    const double cx = 10.0, cy = 6.0, rx = 9.0, ry = 6.0;
    forEachCell(w, h, [&](int x, int y) {
        const double nx = (x - cx) / rx;
        const double ny = (y - cy) / ry;
        const double d = nx * nx + ny * ny;
        if (d <= 1.0) {
            const double light = 1.0 - 0.4 * (nx + ny) - 0.3 * d;
            put(x, y, light > 1.0 ? '@' : light > 0.7 ? '#' : '+');
        }
    });
    // Треугольник: точка внутри, если она по одну сторону
    // от всех трёх рёбер.
    struct P {
        int x;
        int y;
    };
    const P a{22, 11}, b{29, 1}, q{35, 11};
    const auto cross = [](P p0, P p1, int x, int y) {
        return (p1.x - p0.x) * (y - p0.y) -
               (p1.y - p0.y) * (x - p0.x);
    };
    forEachCell(w, h, [&](int x, int y) {
        const int c1 = cross(a, b, x, y);
        const int c2 = cross(b, q, x, y);
        const int c3 = cross(q, a, x, y);
        const bool neg = c1 < 0 || c2 < 0 || c3 < 0;
        const bool pos = c1 > 0 || c2 > 0 || c3 > 0;
        if (!(neg && pos)) {
            put(x, y, '^');
        }
    });
    for (const std::string& row : pic) {
        std::cout << row << '\n';
    }
}
