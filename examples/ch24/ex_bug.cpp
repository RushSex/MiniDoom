#include <array>
#include <iostream>
#include <stack>

struct Cell {
    int x;
    int y;
};

// Пустая комната 4 × 4, цель в углу (0, 0).
int main() {
    std::array<std::array<int, 4>, 4> dist{};
    for (auto& row : dist) {
        row.fill(-1);
    }
    std::stack<Cell> frontier;              // стопка вместо очереди
    dist[0][0] = 0;
    frontier.push({0, 0});
    while (!frontier.empty()) {
        const Cell c = frontier.top();
        frontier.pop();
        const std::array<Cell, 4> next{
            {{c.x + 1, c.y}, {c.x - 1, c.y}, {c.x, c.y + 1},
             {c.x, c.y - 1}}};
        for (const Cell n : next) {
            if (n.x < 0 || n.y < 0 || n.x > 3 || n.y > 3) {
                continue;
            }
            auto& d = dist[static_cast<std::size_t>(n.y)]
                          [static_cast<std::size_t>(n.x)];
            if (d == -1) {
                d = dist[static_cast<std::size_t>(c.y)]
                        [static_cast<std::size_t>(c.x)] + 1;
                frontier.push(n);
            }
        }
    }
    for (const auto& row : dist) {          // правильно: x + y
        for (int d : row) {
            std::cout << ' ' << d;
        }
        std::cout << '\n';
    }
}
