// map.hpp — карта уровня: сетка клеток
#pragma once

#include <optional>
#include <span>
#include <string_view>
#include <vector>

namespace doom {

struct Cell {
    int x = 0;
    int y = 0;
};

class Map {
public:
    explicit Map(std::span<const std::string_view> rows);

    [[nodiscard]] int width() const { return width_; }
    [[nodiscard]] int height() const { return height_; }
    // Клетка карты; всё за краем считается стеной.
    [[nodiscard]] char at(int x, int y) const;
    [[nodiscard]] bool isWall(int x, int y) const;
    // Первая клетка с символом c, если такая есть.
    [[nodiscard]] std::optional<Cell> find(char c) const;

private:
    int width_;
    int height_;
    std::vector<char> cells_;
};

} // namespace doom
