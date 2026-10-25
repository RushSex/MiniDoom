// game.cpp — как устроен мир изнутри
#include "game.hpp"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <string_view>

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
} // namespace

Game::Game(unsigned seed) : rng_(seed) {
    for (int y = 0; y < kHeight; ++y) {
        for (int x = 0; x < kWidth; ++x) {
            const Pos p{x, y};
            switch (cellAt(p)) {
            case '@':
                pos_ = p;
                break;
            case 'i':
                monsters_.push_back({Kind::Imp, p, 20});
                break;
            case 'z':
                monsters_.push_back({Kind::Zombie, p, 10});
                break;
            case 'h':
                medkits_.push_back(p);
                break;
            default:
                break;
            }
        }
    }
}

std::size_t Game::monsterCount() const { return monsters_.size(); }
std::size_t Game::medkitCount() const { return medkits_.size(); }

void Game::command(char key) {
    Pos to = pos_;
    switch (key) {
    case 'w': --to.y; break;
    case 's': ++to.y; break;
    case 'a': --to.x; break;
    case 'd': ++to.x; break;
    default: return;
    }
    if (Monster* m = monsterAt(to)) {
        shoot(*m);
    } else if (cellAt(to) != '#') {
        pos_ = to;
    }
    pickMedkits();
    monstersAttack();
    won_ = cellAt(pos_) == 'X';
}

Monster* Game::monsterAt(Pos p) {
    for (Monster& m : monsters_) {
        if (m.health > 0 && m.pos == p) {
            return &m;
        }
    }
    return nullptr;
}

void Game::shoot(Monster& m) {
    std::uniform_int_distribution<int> shot(5, 15);
    const int dmg = shot(rng_);
    m.health -= dmg;
    std::cout << "Hit " << nameOf(m.kind) << " -" << dmg;
    if (m.health <= 0) {
        std::cout << ", dead";
        ++kills_;
    }
    std::cout << ". ";
}

void Game::pickMedkits() {
    for (Pos& kit : medkits_) {
        if (kit == pos_) {
            health_ = std::min(100, health_ + 25);
            kit = Pos{-1, -1};
            std::cout << "Medkit! ";
        }
    }
}

void Game::monstersAttack() {
    std::uniform_int_distribution<int> bite(2, 8);
    for (const Monster& m : monsters_) {
        const int dist = std::abs(m.pos.x - pos_.x) +
                         std::abs(m.pos.y - pos_.y);
        if (m.health > 0 && dist == 1) {
            const int dmg = bite(rng_);
            health_ -= dmg;
            std::cout << nameOf(m.kind) << " bites -" << dmg << ". ";
        }
    }
}

} // namespace doom
