// app.hpp — вся программа целиком: экраны и переходы между ними
#pragma once

#include "canvas.hpp"
#include "fire.hpp"
#include "game.hpp"
#include "input.hpp"
#include "levels.hpp"
#include "sound.hpp"

#include <cstddef>
#include <random>
#include <string_view>
#include <vector>

namespace doom {

enum class Mode { Title, Playing, Paused, Summary, Credits };

[[nodiscard]] std::string_view modeName(Mode mode);

class App {
public:
    // Уровни по порядку; пустой список — LevelError.
    explicit App(std::vector<Level> levels);

    void update(const Input& in, double dt);
    void render(Canvas& canvas, int fps) const;

    [[nodiscard]] Mode mode() const { return mode_; }
    // Сразу на уровень номер index (с нуля), минуя титульник.
    void warp(std::size_t index);
    [[nodiscard]] std::vector<SoundEvent> takeSounds();
    // Только для тестов: добраться до игры напрямую.
    [[nodiscard]] Game& game() { return game_; }
    [[nodiscard]] const Level& level() const {
        return levels_[current_];
    }

private:
    void enter(Mode mode);              // сменить экран
    void startLevel(std::size_t index, Loadout loadout);
    void drawTitle(Canvas& canvas) const;
    void drawSummary(Canvas& canvas) const;
    void drawCredits(Canvas& canvas) const;

    Mode mode_ = Mode::Title;
    double modeTime_ = 0.0;             // сколько секунд на экране
    std::vector<Level> levels_;         // объявлен раньше game_!
    std::size_t current_ = 0;           // какой уровень идёт
    Game game_;
    Fire fire_{160, 100};
    double fireClock_ = 0.0;
    std::mt19937 rng_{1993};            // год выхода Doom
    std::vector<SoundEvent> sounds_;    // звуки самих экранов
};

} // namespace doom
