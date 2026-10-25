// world.cpp — карта, загрузка уровня, аптечки и атаки монстров
#include "world.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <iostream>

namespace doom {

namespace {
constexpr int kWidth = 12;
constexpr int kHeight = 5;
constexpr std::array<std::string_view, kHeight> kLevel{
    "############",
    "#@..#.z.h.X#",
    "#.#.#.###..#",
    "#h..i......#",
    "############",
};
} // namespace

char cellAt(Pos p) {
    const auto row = static_cast<std::size_t>(p.y);
    const auto col = static_cast<std::size_t>(p.x);
    return kLevel[row][col];
}

std::string_view nameOf(Kind kind) {
    switch (kind) {
    case Kind::Imp:    return "imp";
    case Kind::Zombie: return "zombie";
    }
    return "???";
}

void loadLevel(Player& player, std::vector<Monster>& monsters,
               std::vector<Pos>& medkits) {
    for (int y = 0; y < kHeight; ++y) {
        for (int x = 0; x < kWidth; ++x) {
            const Pos p{x, y};
            switch (cellAt(p)) {
            case '@':
                player.pos = p;
                break;
            case 'i':
                monsters.push_back({Kind::Imp, p, 20});
                break;
            case 'z':
                monsters.push_back({Kind::Zombie, p, 10});
                break;
            case 'h':
                medkits.push_back(p);
                break;
            default:
                break;
            }
        }
    }
}

Monster* monsterAt(std::vector<Monster>& monsters, Pos p) {
    for (Monster& m : monsters) {
        if (m.health > 0 && m.pos == p) {
            return &m;
        }
    }
    return nullptr;
}

void pickMedkits(Player& player, std::vector<Pos>& medkits) {
    for (Pos& kit : medkits) {
        if (kit == player.pos) {
            player.health = std::min(100, player.health + 25);
            kit = Pos{-1, -1};          // подобрали: убираем с карты
            std::cout << "Medkit! ";
        }
    }
}

void monstersAttack(Player& player,
                    const std::vector<Monster>& monsters,
                    std::mt19937& rng) {
    std::uniform_int_distribution<int> bite(2, 8);
    for (const Monster& m : monsters) {
        const int dist = std::abs(m.pos.x - player.pos.x) +
                         std::abs(m.pos.y - player.pos.y);
        if (m.health > 0 && dist == 1) {    // стоит вплотную
            const int dmg = bite(rng);
            player.health -= dmg;
            std::cout << nameOf(m.kind) << " bites -" << dmg << ". ";
        }
    }
}

} // namespace doom
