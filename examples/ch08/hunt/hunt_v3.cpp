// hunt.cpp — в программе спрятаны три бага. Найди их санитайзером.
#include <iostream>
#include <vector>

struct Monster {
    int health = 0;
};

int main() {
    std::vector<Monster> wave{{60}, {20}, {20}};
    wave.push_back({60});                 // подкрепление
    Monster& boss = wave[0];              // ссылка — после роста

    for (std::size_t i = 0; i < wave.size(); ++i) {
        wave[i].health -= 15;             // взрыв бочки
    }
    boss.health -= 30;                    // добиваем босса

    Monster* bonus = new Monster{100};    // бонусный монстр
    std::cout << "boss: " << boss.health << ", bonus: "
              << bonus->health << '\n';
}
