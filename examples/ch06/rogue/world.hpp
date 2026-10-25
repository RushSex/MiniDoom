// world.hpp — типы мира и функции для работы с ним
#pragma once

#include <random>
#include <string_view>
#include <vector>

namespace doom {

enum class Kind { Imp, Zombie };

struct Pos {
    int x = 0;
    int y = 0;
    bool operator==(const Pos&) const = default;
};

struct Monster {
    Kind kind = Kind::Imp;
    Pos pos;
    int health = 0;
};

struct Player {
    Pos pos;
    int health = 100;
    int kills = 0;
};

[[nodiscard]] char cellAt(Pos p);
[[nodiscard]] std::string_view nameOf(Kind kind);

void loadLevel(Player& player, std::vector<Monster>& monsters,
               std::vector<Pos>& medkits);

// Живой монстр в клетке p или nullptr, если там пусто.
[[nodiscard]] Monster* monsterAt(std::vector<Monster>& monsters,
                                Pos p);

void pickMedkits(Player& player, std::vector<Pos>& medkits);
void monstersAttack(Player& player,
                    const std::vector<Monster>& monsters,
                    std::mt19937& rng);

} // namespace doom
