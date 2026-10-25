#include <cmath>
#include <iostream>

struct Vec {
    double x;
    double y;
};

// Направление «вправо» для взгляда (dirX, dirY).
Vec rightOf(Vec dir) {
    return {dir.y, -dir.x};
}

int main() {
    const Vec east{1.0, 0.0};
    const Vec r = rightOf(east);
    std::cout << "facing east, D moves to (" << r.x << ", " << r.y
              << ")\n";
}
