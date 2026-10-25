// main.cpp — игровой цикл рогалика
#include "world.hpp"

#include <iostream>
#include <random>
#include <string>

int main() {
    using namespace doom;
    std::mt19937 rng(5);
    std::uniform_int_distribution<int> shot(5, 15);
    Player player;
    std::vector<Monster> monsters;
    std::vector<Pos> medkits;
    loadLevel(player, monsters, medkits);
    std::cout << "Monsters: " << monsters.size()
              << ", medkits: " << medkits.size() << '\n';

    std::string line;
    while (player.health > 0) {
        std::cout << "[HP " << player.health << "] > ";
        if (!std::getline(std::cin, line)) {
            return 0;
        }
        for (char key : line) {
            Pos to = player.pos;
            switch (key) {
            case 'w': --to.y; break;
            case 's': ++to.y; break;
            case 'a': --to.x; break;
            case 'd': ++to.x; break;
            default: continue;
            }
            if (Monster* m = monsterAt(monsters, to)) {   // атака
                const int dmg = shot(rng);
                m->health -= dmg;
                std::cout << "Hit " << nameOf(m->kind) << " -"
                          << dmg;
                if (m->health <= 0) {
                    std::cout << ", dead";
                    ++player.kills;
                }
                std::cout << ". ";
            } else if (cellAt(to) != '#') {               // шаг
                player.pos = to;
            }
            pickMedkits(player, medkits);
            monstersAttack(player, monsters, rng);
            if (cellAt(player.pos) == 'X') {
                std::cout << "\nEXIT! Kills: " << player.kills << '/'
                          << monsters.size() << '\n';
                return 0;
            }
        }
        std::cout << "@(" << player.pos.x << ','
                  << player.pos.y << ")\n";
    }
    std::cout << "You died.\n";
}
