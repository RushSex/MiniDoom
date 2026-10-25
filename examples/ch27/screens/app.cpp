// app.cpp — машина состояний экранов: титульник, игра, пауза, итоги
#include "app.hpp"

#include "hud.hpp"
#include "shade.hpp"
#include "text.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <string>
#include <utility>

namespace doom {

namespace {
constexpr double kFireStep = 1.0 / 30.0;   // огонь: 30 шагов в с
constexpr double kCount = 1.5;             // итоги считаем 1,5 с
constexpr double kCreditsSpeed = 40.0;     // титры: пикселей в с

constexpr std::array<std::string_view, 9> kCredits{
    "MINI DOOM", "", "CODE", "YOU", "", "LANGUAGE", "C++20", "",
    "THANKS FOR PLAYING"};

void centered(Canvas& c, int y, std::string_view s,
              std::uint32_t color, int scale) {
    drawShadowText(c, (c.width() - textWidth(s, scale)) / 2, y, s,
                   color, scale);
}

// Затемнить весь кадр: k = 256 — как есть, 0 — чёрный.
void dim(Canvas& c, std::uint32_t k) {
    for (int y = 0; y < c.height(); ++y) {
        for (int x = 0; x < c.width(); ++x) {
            c.put(x, y, shade(c.at(x, y), k));
        }
    }
}
} // namespace

std::string_view modeName(Mode mode) {
    switch (mode) {
    case Mode::Title: return "Title";
    case Mode::Playing: return "Playing";
    case Mode::Paused: return "Paused";
    case Mode::Summary: return "Summary";
    case Mode::Credits: return "Credits";
    }
    return "?";
}

App::App() {
    for (int i = 0; i < 100; ++i) {       // разжечь огонь заранее
        fire_.step(rng_);
    }
}

void App::enter(Mode mode) {
    mode_ = mode;
    modeTime_ = 0.0;
    sounds_.push_back({SoundId::Pickup, 0.6f});   // щелчок меню
}

void App::update(const Input& in, double dt) {
    modeTime_ += dt;
    switch (mode_) {
    case Mode::Title:
        fireClock_ += dt;
        while (fireClock_ >= kFireStep) {  // у огня свой темп
            fireClock_ -= kFireStep;
            fire_.step(rng_);
        }
        if (in.confirm) {
            game_ = Game{Map{kLevel}};     // новая игра с нуля
            enter(Mode::Playing);
        }
        break;
    case Mode::Playing:
        if (in.pause) {
            enter(Mode::Paused);
            break;
        }
        game_.update(in, dt);
        if (game_.levelDone()) {
            enter(Mode::Summary);
        }
        break;
    case Mode::Paused:                 // мир стоит: update не зовём
        if (in.pause || in.confirm) {
            enter(Mode::Playing);
        }
        break;
    case Mode::Summary:
        if (in.confirm) {
            if (modeTime_ < kCount) {
                modeTime_ = kCount;    // не ждать — досчитать сразу
            } else {
                enter(Mode::Credits);
            }
        }
        break;
    case Mode::Credits: {
        const double end =
            (static_cast<double>(kCredits.size()) * 40.0 + 400.0) /
            kCreditsSpeed;
        if (in.confirm || modeTime_ > end) {
            enter(Mode::Title);
        }
        break;
    }
    }
}

std::vector<SoundEvent> App::takeSounds() {
    std::vector<SoundEvent> out = std::exchange(sounds_, {});
    for (const SoundEvent& e : game_.takeSounds()) {
        out.push_back(e);
    }
    return out;
}

void App::render(Canvas& canvas, int fps) const {
    switch (mode_) {
    case Mode::Title:
        drawTitle(canvas);
        break;
    case Mode::Playing:
        game_.render(canvas);
        drawHud(canvas, game_, fps);
        break;
    case Mode::Paused:
        game_.render(canvas);
        drawHud(canvas, game_, fps);
        dim(canvas, 100);
        centered(canvas, 140, "PAUSED", rgb(255, 255, 255), 7);
        centered(canvas, 220, "ESC - BACK TO HELL",
                 rgb(220, 200, 160), 2);
        break;
    case Mode::Summary:
        drawSummary(canvas);
        break;
    case Mode::Credits:
        drawCredits(canvas);
        break;
    }
}

void App::drawTitle(Canvas& canvas) const {
    canvas.clear(rgb(7, 7, 7));
    fire_.draw(canvas, 4);
    centered(canvas, 80, "MINI DOOM", rgb(200, 30, 20), 10);
    centered(canvas, 160, "C++20 EDITION", rgb(255, 220, 160), 2);
    if (static_cast<int>(modeTime_ * 2.0) % 2 == 0) {   // мигает
        centered(canvas, 300, "PRESS ENTER", rgb(255, 255, 255), 3);
    }
}

void App::drawSummary(Canvas& canvas) const {
    canvas.clear(rgb(40, 8, 6));
    centered(canvas, 50, "LEVEL COMPLETE", rgb(255, 200, 60), 5);
    // Проценты «досчитываются» на глазах, как в Doom.
    const double k = std::min(1.0, modeTime_ / kCount);
    const auto percent = [k](int got, int total) {
        const int full = total == 0 ? 100 : got * 100 / total;
        return static_cast<int>(full * k);
    };
    const int kills = percent(game_.kills(), game_.monstersTotal());
    const int items =
        percent(game_.itemsTaken(), game_.itemsTotal());
    const auto secs = static_cast<int>(game_.time() * k);
    using Row = std::pair<std::string_view, std::string>;
    const std::array<Row, 3> rows{
        {{"KILLS", std::format("{}%", kills)},
         {"ITEMS", std::format("{}%", items)},
         {"TIME", std::format("{}:{:02}", secs / 60, secs % 60)}}};
    for (std::size_t i = 0; i < rows.size(); ++i) {
        const int y = 140 + static_cast<int>(i) * 50;
        drawShadowText(canvas, 150, y, rows[i].first,
                       rgb(230, 230, 230), 4);
        const std::string& v = rows[i].second;
        drawShadowText(canvas, 490 - textWidth(v, 4), y, v,
                       rgb(220, 40, 30), 4);
    }
    if (modeTime_ >= kCount) {
        centered(canvas, 330, "PRESS ENTER", rgb(255, 255, 255), 2);
    }
}

void App::drawCredits(Canvas& canvas) const {
    canvas.clear(rgb(0, 0, 0));
    // Строки едут снизу вверх: y уменьшается со временем.
    const int top = canvas.height() -
                    static_cast<int>(modeTime_ * kCreditsSpeed);
    for (std::size_t i = 0; i < kCredits.size(); ++i) {
        const int y = top + static_cast<int>(i) * 40;
        if (y > -40 && y < canvas.height()) {
            const auto color = i == 0 ? rgb(200, 30, 20)
                                      : rgb(220, 220, 220);
            centered(canvas, y, kCredits[i], color, i == 0 ? 6 : 3);
        }
    }
}

} // namespace doom
