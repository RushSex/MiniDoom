#include <iostream>
#include <vector>

struct Monster {
    int health = 20;
};

int main() {
    std::vector<Monster> wave{{60}, {20}};
    Monster& boss = wave[0];          // ссылка на первого монстра
    wave.push_back({20});             // подкрепление прибыло
    boss.health -= 10;                // бьём босса... куда?
    std::cout << "boss: " << boss.health << '\n';
}
