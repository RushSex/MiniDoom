// items.hpp — предметы, которые лежат на уровне
#pragma once

#include <cstdint>

namespace doom {

enum class ItemKind : std::uint8_t { Medkit, Clip };

struct Item {
    ItemKind kind;
    double x = 0.0;          // центр клетки, где лежит
    double y = 0.0;
    bool taken = false;
};

} // namespace doom
