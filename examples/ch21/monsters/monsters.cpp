// monsters.cpp — имп и зомби из эллипсов и многоугольников
#include "monsters.hpp"

#include "canvas.hpp"
#include "paint.hpp"

#include <array>
#include <cstddef>

namespace doom {

namespace {
// Прямоугольник — частный случай многоугольника.
void rect(Image& img, double x0, double y0, double x1, double y1,
          std::uint32_t c) {
    const std::array<Point, 4> p{{{x0, y0}, {x1, y0}, {x1, y1},
                                  {x0, y1}}};
    fillPolygon(img, p, c);
}

void tri(Image& img, Point a, Point b, Point c, std::uint32_t col) {
    const std::array<Point, 3> p{a, b, c};
    fillPolygon(img, p, col);
}

// Шаг ноги: в двух позах ноги сдвинуты в разные стороны.
double stepOf(Pose pose) {
    return pose == Pose::Walk1 ? -3.0 : 3.0;
}
} // namespace

Image makeImp(Pose pose) {
    Image img(64, 64);
    const auto skin = rgb(150, 95, 55);
    const auto dark = rgb(95, 55, 30);
    const auto bone = rgb(225, 215, 185);
    const double s = stepOf(pose);
    rect(img, 24 + s, 46, 29 + s, 63, dark);             // ноги
    rect(img, 35 - s, 46, 40 - s, 63, dark);
    fillEllipse(img, {32, 37}, 11, 12, skin);            // туловище
    fillEllipse(img, {32, 34}, 6, 5, rgb(175, 125, 80)); // грудь
    fillEllipse(img, {20, 40 - s / 2}, 3.5, 9, skin);    // руки
    fillEllipse(img, {44, 40 + s / 2}, 3.5, 9, skin);
    tri(img, {19, 22}, {23, 30}, {26, 28}, bone);        // шипы
    tri(img, {45, 22}, {41, 30}, {38, 28}, bone);
    fillEllipse(img, {32, 20}, 7, 8, skin);              // голова
    tri(img, {26, 15}, {28, 7}, {30, 14}, bone);         // рога
    tri(img, {34, 14}, {36, 7}, {38, 15}, bone);
    rect(img, 27, 19, 29, 21, rgb(255, 200, 30));        // глаза
    rect(img, 35, 19, 37, 21, rgb(255, 200, 30));
    rect(img, 28, 24, 36, 26, rgb(40, 10, 5));           // пасть
    outline(img, rgb(25, 12, 6));
    return img;
}

Image makeZombie(Pose pose) {
    Image img(64, 64);
    const auto cloth = rgb(75, 105, 55);
    const auto gun = rgb(55, 55, 60);
    const double s = stepOf(pose);
    rect(img, 25 + s, 46, 30 + s, 63, rgb(60, 40, 25));  // сапоги
    rect(img, 34 - s, 46, 39 - s, 63, rgb(60, 40, 25));
    fillEllipse(img, {32, 37}, 10, 12, cloth);           // туловище
    rect(img, 22, 44, 42, 46, rgb(50, 40, 30));          // ремень
    fillEllipse(img, {22, 37}, 3.5, 8, cloth);           // руки
    fillEllipse(img, {42, 37}, 3.5, 8, cloth);
    const std::array<Point, 4> rifle{{{19, 42}, {45, 30}, {46, 34},
                                      {20, 46}}};
    fillPolygon(img, rifle, gun);                        // винтовка
    fillEllipse(img, {32, 20}, 6.5, 7.5, rgb(205, 155, 120)); // лицо
    rect(img, 26, 13, 38, 15, rgb(50, 45, 40));          // стрижка
    rect(img, 28, 19, 30, 21, rgb(200, 20, 20));         // глаза
    rect(img, 34, 19, 36, 21, rgb(200, 20, 20));
    outline(img, rgb(15, 20, 10));
    return img;
}

const Image& monsterImage(MonsterKind kind, Pose pose) {
    // Четыре кадра: [вид][поза]. Лямбда строит их при первом вызове.
    static const auto frames = [] {
        return std::array<std::array<Image, 2>, 2>{{
            {makeImp(Pose::Walk1), makeImp(Pose::Walk2)},
            {makeZombie(Pose::Walk1), makeZombie(Pose::Walk2)},
        }};
    }();
    const auto k = static_cast<std::size_t>(kind);
    const auto p = static_cast<std::size_t>(pose);
    return frames[k][p];
}

} // namespace doom
