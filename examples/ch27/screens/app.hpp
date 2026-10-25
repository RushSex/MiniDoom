// app.hpp — вся программа целиком: экраны и переходы между ними
#pragma once

#include "canvas.hpp"
#include "fire.hpp"
#include "game.hpp"
#include "input.hpp"
#include "level.hpp"
#include "sound.hpp"

#include <random>
#include <string_view>
#include <vector>

namespace doom {

enum class Mode { Title, Playing, Paused, Summary, Credits };

[[nodiscard]] std::string_view modeName(Mode mode);

class App {
public:
    App();

    void update(const Input& in, double dt);
    void render(Canvas& canvas, int fps) const;

    [[nodiscard]] Mode mode() const { return mode_; }
    [[nodiscard]] std::vector<SoundEvent> takeSounds();
    // Только для тестов: добраться до игры напрямую.
    [[nodiscard]] Game& game() { return game_; }

private:
    void enter(Mode mode);              // сменить экран
    void drawTitle(Canvas& canvas) const;
    void drawSummary(Canvas& canvas) const;
    void drawCredits(Canvas& canvas) const;

    Mode mode_ = Mode::Title;
    double modeTime_ = 0.0;             // сколько секунд на экране
    Game game_{Map{kLevel}};
    Fire fire_{160, 100};
    double fireClock_ = 0.0;
    std::mt19937 rng_{1993};            // год выхода Doom
    std::vector<SoundEvent> sounds_;    // звуки самих экранов
};

} // namespace doom
