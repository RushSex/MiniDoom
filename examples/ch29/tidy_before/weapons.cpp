// weapons.cpp — пистолет и дробовик, нарисованные кистями
#include "weapons.hpp"

#include "paint.hpp"

#include <array>

namespace doom {

namespace {
void quad(Image& img, Point a, Point b, Point c, Point d,
          std::uint32_t col) {
    const std::array<Point, 4> p{a, b, c, d};
    fillPolygon(img, p, col);
}

// Вспышка: несколько эллипсов от жёлтого к белому.
void flash(Image& img, Point c, double r) {
    fillEllipse(img, c, r, r * 0.8, rgb(230, 90, 20));
    fillEllipse(img, c, r * 0.7, r * 0.55, rgb(255, 200, 60));
    fillEllipse(img, c, r * 0.35, r * 0.3, rgb(255, 255, 220));
}
} // namespace

Image makeGun(Weapon w, bool firing) {
    Image img(120, 80);
    const auto metal = rgb(70, 72, 78);
    if (w == Weapon::Shotgun) {
        if (firing) {
            flash(img, {60, 12}, 18);
        }
        quad(img, {49, 12}, {59, 12}, {58, 70}, {46, 70}, metal);
        quad(img, {61, 12}, {71, 12}, {74, 70}, {62, 70}, metal);
        quad(img, {42, 38}, {78, 38}, {82, 60}, {38, 60},
             rgb(125, 80, 40));                      // цевьё
    } else {
        if (firing) {
            flash(img, {60, 22}, 13);
        }
        quad(img, {52, 26}, {68, 26}, {74, 66}, {46, 66}, metal);
        quad(img, {59, 21}, {61, 21}, {61, 26}, {59, 26},
             rgb(40, 40, 40));                       // мушка
    }
    quad(img, {42, 62}, {78, 62}, {86, 79}, {34, 79},
         rgb(200, 150, 115));                        // кисть руки
    quad(img, {38, 74}, {82, 74}, {88, 79}, {32, 79},
         rgb(70, 95, 55));                           // рукав
    outline(img, rgb(20, 18, 16));
    return img;
}

void drawWeapon(Canvas& canvas, Weapon w, bool firing, int dx,
                int dy) {
    // Четыре картинки: [оружие][выстрел], строятся один раз.
    static const auto images = [] {
        return std::array<Image, 4>{makeGun(Weapon::Pistol, false),
                                    makeGun(Weapon::Pistol, true),
                                    makeGun(Weapon::Shotgun, false),
                                    makeGun(Weapon::Shotgun, true)};
    }();
    const std::size_t shot = firing ? 1 : 0;
    const auto wi = static_cast<std::size_t>(w);
    const Image& img = images[wi * 2 + shot];
    constexpr int kScale = 2;             // пиксель картинки — 2 × 2
    const int left = canvas.width() / 2 - img.width() + dx;
    const int top = canvas.height() - img.height() * kScale + 6 + dy;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            const std::uint32_t c = img.at(x, y);
            if ((c >> 24) != 0) {
                canvas.fillRect(left + x * kScale, top + y * kScale,
                                kScale, kScale, c & 0xFFFFFF);
            }
        }
    }
}

} // namespace doom
