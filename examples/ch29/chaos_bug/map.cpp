// map.cpp — карта уровня и двери
#include "map.hpp"

namespace doom {

Map::Map(std::span<const std::string_view> rows)
    : width_(static_cast<int>(rows.front().size())),
      height_(static_cast<int>(rows.size())) {
    for (std::string_view row : rows) {
        cells_.insert(cells_.end(), row.begin(), row.end());
    }
    doors_.resize(cells_.size());
}

std::size_t Map::index(int x, int y) const {
    return cellIndex(x, y, width_);
}

char Map::at(int x, int y) const {
    if (x < 0 || y < 0 || x >= width_ || y >= height_) {
        return '1';             // за краем карты — сплошная стена
    }
    return cells_[index(x, y)];
}

bool Map::isWall(int x, int y) const {
    const char c = at(x, y);
    return c >= '1' && c <= '9';
}

bool Map::isSolid(int x, int y) const {
    if (const Door* d = door(x, y)) {
        return d->open < 0.9;   // пройти можно, только когда открыта
    }
    return isWall(x, y);
}

Door* Map::door(int x, int y) {
    return at(x, y) == 'D' ? &doors_[index(x, y)] : nullptr;
}

const Door* Map::door(int x, int y) const {
    return at(x, y) == 'D' ? &doors_[index(x, y)] : nullptr;
}

std::optional<Cell> Map::find(char c) const {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            if (at(x, y) == c) {
                return Cell{x, y};
            }
        }
    }
    return std::nullopt;
}

} // namespace doom
