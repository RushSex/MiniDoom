// map.hpp — карта уровня: сетка клеток и двери
#pragma once

#include <cstddef>
#include <optional>
#include <span>
#include <string_view>
#include <vector>

namespace doom {

struct Cell {
    int x = 0;
    int y = 0;
};

// Дверь: плита посреди клетки, которая уезжает вбок.
struct Door {
    double open = 0.0;   // 0 — закрыта, 1 — открыта полностью
    double wait = 0.0;   // сколько ещё секунд стоять открытой
    int move = 0;        // +1 открывается, −1 закрывается, 0 стоит
};

class Map {
public:
    explicit Map(std::span<const std::string_view> rows);

    [[nodiscard]] int width() const { return width_; }
    [[nodiscard]] int height() const { return height_; }
    // Клетка карты; всё за краем считается стеной.
    [[nodiscard]] char at(int x, int y) const;
    [[nodiscard]] bool isWall(int x, int y) const;
    // Сквозь клетку нельзя пройти: стена или почти закрытая дверь.
    [[nodiscard]] bool isSolid(int x, int y) const;
    // Дверь в клетке (x, y) или nullptr, если там не дверь.
    [[nodiscard]] Door* door(int x, int y);
    [[nodiscard]] const Door* door(int x, int y) const;
    // Первая клетка с символом c, если такая есть.
    [[nodiscard]] std::optional<Cell> find(char c) const;

private:
    [[nodiscard]] std::size_t index(int x, int y) const;

    int width_;
    int height_;
    std::vector<char> cells_;
    std::vector<Door> doors_;   // по одной на клетку; нужны у D
};

} // namespace doom
