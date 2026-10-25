// game.hpp — весь мир рогалика в одном классе
#pragma once

#include <cstddef>
#include <random>
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

class Game {
public:
    explicit Game(unsigned seed);

    void command(char key);          // один шаг: w, a, s, d

    [[nodiscard]] bool won() const { return won_; }
    [[nodiscard]] bool lost() const { return health_ <= 0; }
    [[nodiscard]] int health() const { return health_; }
    [[nodiscard]] int kills() const { return kills_; }
    [[nodiscard]] Pos position() const { return pos_; }
    [[nodiscard]] std::size_t monsterCount() const;
    [[nodiscard]] std::size_t medkitCount() const;

private:
    [[nodiscard]] Monster* monsterAt(Pos p);
    void shoot(Monster& m);
    void pickMedkits();
    void monstersAttack();

    std::mt19937 rng_;
    Pos pos_;
    int health_ = 100;
    int kills_ = 0;
    bool won_ = false;
    std::vector<Monster> monsters_;
    std::vector<Pos> medkits_;
};

} // namespace doom
