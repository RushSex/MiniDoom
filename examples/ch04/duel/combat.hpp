// combat.hpp — интерфейс боя: ЧТО умеет модуль, без подробностей КАК
#pragma once

#include <random>

namespace doom {

// Печатает строку состояния хода.
void printStatus(int turn, int playerHealth, int impHealth,
                 int medkits);

// Читает ход игрока. Ввод кончился или сломан — '3' (сбежать).
[[nodiscard]] char readAction();

// Выстрел из пистолета по импу. Возвращает нанесённый урон.
[[nodiscard]] int shoot(int& impHealth, std::mt19937& rng);

// Аптечка: +25, но не выше 100. Возвращает false, если аптечек нет.
[[nodiscard]] bool useMedkit(int& playerHealth, int& medkits);

// Ход импа. Возвращает урон по игроку, 0 — промах.
[[nodiscard]] int impAttack(int& playerHealth, std::mt19937& rng);

} // namespace doom
