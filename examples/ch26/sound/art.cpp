// art.cpp — аптечка и коробка патронов: прямоугольники с альфой
#include "art.hpp"

#include "canvas.hpp"
#include "paint.hpp"

#include <array>

namespace doom {

namespace {
// Прямоугольник [x0, x1] × [y0, y1] на картинке спрайта.
void box(Image& img, int x0, int y0, int x1, int y1,
         std::uint32_t c) {
    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            img.set(x, y, opaque(c));
        }
    }
}
} // namespace

Image makeMedkit() {
    Image img(64, 64);                // всё прозрачное: нули
    box(img, 20, 47, 43, 63, rgb(40, 40, 40));       // контур
    box(img, 21, 48, 42, 62, rgb(225, 225, 220));    // коробка
    box(img, 29, 50, 34, 60, rgb(210, 20, 20));      // красный
    box(img, 25, 53, 38, 57, rgb(210, 20, 20));      // крест
    return img;
}

Image makeClip() {
    Image img(64, 64);
    box(img, 21, 51, 42, 63, rgb(20, 18, 12));
    box(img, 22, 52, 41, 62, rgb(80, 70, 50));       // коробка
    for (int i = 0; i < 5; ++i) {                    // пять патронов
        const int x = 24 + i * 4;
        box(img, x, 44, x + 2, 51, rgb(200, 160, 60));
        box(img, x, 42, x + 2, 43, rgb(150, 100, 40));
    }
    return img;
}

const Image& itemImage(ItemKind kind) {
    static const std::array<Image, 2> images{makeMedkit(),
                                            makeClip()};
    return images[static_cast<std::size_t>(kind)];
}

const Image& fxImage(FxKind kind) {
    static const auto images = [] {
        std::array<Image, 4> im{Image(64, 64), Image(64, 64),
                                Image(64, 64), Image(64, 64)};
        fillEllipse(im[0], {32, 32}, 4, 4, rgb(150, 150, 140));
        fillEllipse(im[0], {29, 29}, 2.5, 2.5, rgb(190, 190, 180));
        fillEllipse(im[1], {32, 32}, 4, 3.5, rgb(170, 10, 10));
        fillEllipse(im[1], {28, 35}, 2, 2, rgb(120, 0, 0));
        for (Image* img : {&im[2], &im[3]}) {     // взрыв и шар
            fillEllipse(*img, {32, 32}, 9, 8, rgb(230, 70, 15));
            fillEllipse(*img, {32, 31}, 6, 5, rgb(255, 190, 50));
            fillEllipse(*img, {32, 30}, 3, 2.5, rgb(255, 255, 210));
        }
        return im;
    }();
    return images[static_cast<std::size_t>(kind)];
}

} // namespace doom
