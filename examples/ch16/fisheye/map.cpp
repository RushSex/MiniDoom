// map.cpp — карта уровня
#include "map.hpp"

namespace doom {

Map::Map(std::span<const std::string_view> rows)
    : width_(static_cast<int>(rows.front().size())),
      height_(static_cast<int>(rows.size())) {
    for (std::string_view row : rows) {
        cells_.insert(cells_.end(), row.begin(), row.end());
    }
}

char Map::at(int x, int y) const {
    if (x < 0 || y < 0 || x >= width_ || y >= height_) {
        return '1';             // за краем карты — сплошная стена
    }
    const auto i = static_cast<std::size_t>(y * width_ + x);
    return cells_[i];
}

bool Map::isWall(int x, int y) const {
    const char c = at(x, y);
    return c >= '1' && c <= '9';
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
