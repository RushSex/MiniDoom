// pathfind.hpp — поле расстояний: сколько шагов до цели
#pragma once

#include "map.hpp"

#include <vector>

namespace doom {

inline constexpr int kUnreached = -1;   // сюда дойти нельзя

// Поиск в ширину от клетки goal. Результат — по числу на клетку,
// индекс y * ширина + x. Стены непроходимы, двери — проходимы:
// монстр откроет дверь сам.
[[nodiscard]] std::vector<int> distanceField(const Map& map,
                                           Cell goal);

} // namespace doom
