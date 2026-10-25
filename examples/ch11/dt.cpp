#include <format>
#include <iostream>

// Секунда игры при заданном FPS: два способа двигать игрока.
void simulate(int fps) {
    const double dt = 1.0 / fps;      // длительность кадра, с
    double perFrame = 0.0;            // «2 пикселя за кадр»
    double perSecond = 0.0;           // «120 пикселей в секунду»
    for (int frame = 0; frame < fps; ++frame) {
        perFrame += 2.0;
        perSecond += 120.0 * dt;
    }
    std::cout << std::format("{:>4} FPS: per frame {:>6.1f} px,"
                             " per second {:>6.1f} px\n",
                             fps, perFrame, perSecond);
}

int main() {
    simulate(30);
    simulate(60);
    simulate(144);
}
