#include <array>
#include <cstddef>
#include <format>
#include <iostream>
#include <queue>
#include <string_view>

// # — стена, G — цель. Найдём, сколько шагов до G из каждой клетки.
constexpr std::array<std::string_view, 5> kMap{
    "#########",
    "#G..#...#",
    "#.#.#.#.#",
    "#.#...#.#",
    "#########",
};
constexpr int kW = 9;
constexpr int kH = 5;

struct Cell {
    int x;
    int y;
};

int main() {
    std::array<std::array<int, kW>, kH> dist{};
    for (auto& row : dist) {
        row.fill(-1);                       // −1 — ещё не были
    }
    std::queue<Cell> frontier;
    dist[1][1] = 0;                         // G стоит в (1, 1)
    frontier.push({1, 1});
    int visits = 0;
    while (!frontier.empty()) {
        const Cell c = frontier.front();
        frontier.pop();
        ++visits;
        const std::array<Cell, 4> next{
            {{c.x + 1, c.y}, {c.x - 1, c.y}, {c.x, c.y + 1},
             {c.x, c.y - 1}}};
        for (const Cell n : next) {
            const auto ux = static_cast<std::size_t>(n.x);
            const auto uy = static_cast<std::size_t>(n.y);
            if (kMap[uy][ux] == '#' || dist[uy][ux] != -1) {
                continue;
            }
            dist[uy][ux] = dist[static_cast<std::size_t>(c.y)]
                               [static_cast<std::size_t>(c.x)] + 1;
            frontier.push(n);
        }
    }
    for (std::size_t y = 0; y < kH; ++y) {
        for (std::size_t x = 0; x < kW; ++x) {
            if (dist[y][x] < 0) {
                std::cout << "  #";
            } else {
                std::cout << std::format("{:3}", dist[y][x]);
            }
        }
        std::cout << '\n';
    }
    std::cout << "cells visited: " << visits << '\n';
}
