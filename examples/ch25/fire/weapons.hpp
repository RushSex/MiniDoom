// weapons.hpp — оружие: характеристики и картинка в руках
#pragma once

#include "canvas.hpp"
#include "image.hpp"

#include <array>
#include <cstdint>

namespace doom {

enum class Weapon : std::uint8_t { Pistol, Shotgun };

struct WeaponSpec {
    double cooldown;   // секунд между выстрелами
    int pellets;       // сколько пуль за выстрел
    double spread;     // разброс, радиан в каждую сторону
    int minDamage;     // урон одной пули: от и до
    int maxDamage;
    int ammo;          // сколько патронов тратит выстрел
};

inline constexpr std::array<WeaponSpec, 2> kWeapons{{
    {0.35, 1, 0.02, 10, 15, 1},     // пистолет: точный
    {0.90, 7, 0.10, 5, 15, 2},      // дробовик: семь дробин веером
}};

[[nodiscard]] constexpr const WeaponSpec& specOf(Weapon w) {
    return kWeapons[static_cast<std::size_t>(w)];
}

// Картинка оружия в руках 120 × 80; firing — со вспышкой.
[[nodiscard]] Image makeGun(Weapon w, bool firing);
// Рисует оружие внизу экрана со сдвигом (dx, dy) — покачивание.
void drawWeapon(Canvas& canvas, Weapon w, bool firing, int dx,
                int dy);

} // namespace doom
