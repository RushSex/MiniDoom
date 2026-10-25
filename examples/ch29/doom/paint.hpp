// paint.hpp — кисти для рисования спрайтов кодом
#pragma once

#include "image.hpp"

#include <cstdint>
#include <span>

namespace doom {

struct Point {
    double x = 0.0;
    double y = 0.0;
};

// Вызывает f(x, y) для каждого пикселя картинки. f — что угодно,
// что можно вызвать с двумя int: функция, лямбда, объект.
template <typename F>
void forEachPixel(Image& img, F f) {
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            f(x, y);
        }
    }
}

// Цвет, умноженный на яркость k (1 — как есть), непрозрачный.
[[nodiscard]] std::uint32_t lit(std::uint32_t base, double k);

// Эллипс с «объёмом»: свет падает слева сверху.
void fillEllipse(Image& img, Point c, double rx, double ry,
                 std::uint32_t base);
// Выпуклый многоугольник: вершины по кругу в любую сторону.
void fillPolygon(Image& img, std::span<const Point> pts,
                 std::uint32_t base);
// Тёмная обводка вокруг всех непрозрачных пикселей.
void outline(Image& img, std::uint32_t base);
// Подмешать цвет ко всей фигуре: amount от 0 до 1.
void tint(Image& img, std::uint32_t base, double amount);

} // namespace doom
