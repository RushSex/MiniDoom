// main.cpp — снаружи класса остался только диалог с игроком
#include "game.hpp"

#include <iostream>
#include <string>

int main() {
    doom::Game game(5);
    std::cout << "Monsters: " << game.monsterCount()
              << ", medkits: " << game.medkitCount() << '\n';

    std::string line;
    while (!game.lost()) {
        std::cout << "[HP " << game.health() << "] > ";
        if (!std::getline(std::cin, line)) {
            return 0;
        }
        for (char key : line) {
            game.command(key);
            if (game.won()) {
                std::cout << "\nEXIT! Kills: " << game.kills() << '/'
                          << game.monsterCount() << '\n';
                return 0;
            }
        }
        const doom::Pos p = game.position();
        std::cout << "@(" << p.x << ',' << p.y << ")\n";
    }
    std::cout << "You died.\n";
}
