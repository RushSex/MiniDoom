// pathfind.cpp — поиск в ширину (BFS) по сетке карты
#include "pathfind.hpp"

#include <array>
#include <cstddef>
#include <queue>

namespace doom {

std::vector<int> distanceField(const Map& map, Cell goal) {
    const int w = map.width();
    const auto index = [w](int x, int y) {
        return cellIndex(x, y, w);
    };
    std::vector<int> dist(index(0, map.height()), kUnreached);
    std::queue<Cell> frontier;              // кого ещё обойти
    dist[index(goal.x, goal.y)] = 0;
    frontier.push(goal);
    constexpr std::array<Cell, 4> kSteps{
        {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
    while (!frontier.empty()) {
        const Cell c = frontier.front();    // самая ранняя в очереди
        frontier.pop();
        for (const Cell s : kSteps) {
            const Cell n{c.x + s.x, c.y + s.y};
            if (map.isWall(n.x, n.y) ||
                dist[index(n.x, n.y)] != kUnreached) {
                continue;                   // стена или уже знаем
            }
            dist[index(n.x, n.y)] = dist[index(c.x, c.y)] + 1;
            frontier.push(n);
        }
    }
    return dist;
}

} // namespace doom
