#include <algorithm>
#include <iostream>
#include <vector>

struct Sprite {
    char id;
    double depth;
};

int main() {
    std::vector<Sprite> sprites{{'A', 2.0}, {'B', 6.0}, {'C', 4.0}};
    std::ranges::sort(sprites, {}, &Sprite::depth);
    std::cout << "draw order:";
    for (const Sprite& s : sprites) {   // рисуем по очереди
        std::cout << ' ' << s.id << '(' << s.depth << ')';
    }
    std::cout << '\n';
}
