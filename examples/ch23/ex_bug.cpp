#include <cmath>
#include <iostream>

// Монстр в (mx, my) идёт к игроку в (px, py) со скоростью
// speed клеток в секунду. Шаг за один кадр длиной dt.
void step(double& mx, double& my, double px, double py,
          double speed, double dt) {
    mx += (px - mx) * speed * dt;
    my += (py - my) * speed * dt;
}

int main() {
    for (double start : {2.0, 10.0}) {
        double mx = start, my = 0.0;
        step(mx, my, 0.0, 0.0, 1.5, 1.0 / 60.0);
        std::cout << "from " << start << ": moved "
                  << start - mx << " in one frame\n";
    }
}
