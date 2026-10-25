// synth.cpp — генераторы, огибающая, фильтр и рецепты звуков
#include "synth.hpp"

#include <algorithm>
#include <cmath>
#include <numbers>
#include <random>

namespace doom {

namespace {
using enum Shape;

// Рецепты в порядке SoundId: два слоя, атака, длина, фильтр.
constexpr std::array<Recipe, kSoundCount> kRecipes{{
    {{{{Noise, 8000, 2000, 1.0}, {Square, 180, 60, 0.5}}},
     0.002, 0.25, 5000},                            // Pistol
    {{{{Noise, 6000, 800, 1.0}, {Saw, 90, 40, 0.7}}},
     0.002, 0.60, 3000},                            // Shotgun
    {{{{Saw, 220, 110, 0.7}, {Noise, 1500, 600, 0.4}}},
     0.05, 0.70, 1800},                             // ImpSight
    {{{{Square, 110, 80, 0.6}, {Noise, 900, 500, 0.4}}},
     0.05, 0.60, 1200},                             // ZombieSight
    {{{{Saw, 400, 250, 0.7}, {Noise, 2000, 1000, 0.3}}},
     0.01, 0.30, 2500},                             // MonsterPain
    {{{{Saw, 300, 60, 0.7}, {Noise, 2500, 300, 0.5}}},
     0.01, 1.00, 2000},                             // MonsterDeath
    {{{{Noise, 3000, 1200, 0.8}, {Sine, 300, 500, 0.3}}},
     0.05, 0.50, 2500},                             // Fireball
    {{{{Noise, 2000, 100, 1.0}, {Sine, 80, 30, 0.8}}},
     0.003, 0.90, 1200},                            // Boom
    {{{{Square, 70, 70, 0.5}, {Noise, 400, 300, 0.6}}},
     0.10, 0.90, 600},                              // Door
    {{{{Square, 660, 1320, 0.5}, {}}},
     0.005, 0.15, 0},                               // Pickup
    {{{{Saw, 300, 200, 0.7}, {Noise, 1500, 800, 0.3}}},
     0.01, 0.35, 2000},                             // PlayerHurt
    {{{{Saw, 260, 70, 0.7}, {Noise, 1200, 200, 0.4}}},
     0.02, 1.40, 1500},                             // PlayerDeath
}};

constexpr std::array<std::string_view, kSoundCount> kNames{
    "pistol", "shotgun", "imp_sight", "zombie_sight",
    "monster_pain", "monster_death", "fireball", "boom",
    "door", "pickup", "player_hurt", "player_death"};

// Громкость во времени: быстро вверх, потом плавно в ноль.
double envelope(double t, double attack, double length) {
    if (t < attack) {
        return t / attack;
    }
    const double u = (t - attack) / (length - attack);
    return (1.0 - u) * (1.0 - u);
}

// Фильтр нижних частот: срезает «писк», оставляет «бас».
void lowpass(Wave& w, double cutoff) {
    if (cutoff <= 0.0) {
        return;
    }
    const double w0 = 2.0 * std::numbers::pi * cutoff / kSampleRate;
    const double a = 1.0 - std::exp(-w0);
    double y = 0.0;
    for (float& s : w) {
        y += a * (s - y);       // догоняем вход на долю a
        s = static_cast<float>(y);
    }
}
} // namespace

std::string_view soundName(SoundId id) {
    return kNames[static_cast<std::size_t>(id)];
}

float oscillator(Shape shape, double phase, float held) {
    switch (shape) {
    case Sine:
        return static_cast<float>(
            std::sin(2.0 * std::numbers::pi * phase));
    case Saw:
        return static_cast<float>(2.0 * phase - 1.0);
    case Square:
        return phase < 0.5 ? 1.0f : -1.0f;
    case Noise:
        return held;
    }
    return 0.0f;
}

Wave synthesize(const Recipe& r, unsigned seed) {
    const auto n = static_cast<std::size_t>(r.length * kSampleRate);
    Wave out(n, 0.0f);
    std::minstd_rand rng(seed);
    std::uniform_real_distribution<float> white(-1.0f, 1.0f);
    for (const Layer& layer : r.layers) {
        if (layer.gain == 0.0) {
            continue;
        }
        double phase = 0.0;
        float held = 0.0f;      // шум меняется раз в период
        for (std::size_t i = 0; i < n; ++i) {
            const double u =
                static_cast<double>(i) / static_cast<double>(n);
            // Частота едет по степени: так её слышит ухо.
            const double freq =
                layer.from * std::pow(layer.to / layer.from, u);
            phase += freq / kSampleRate;
            if (phase >= 1.0) {
                phase -= 1.0;
                held = white(rng);
            }
            out[i] += static_cast<float>(layer.gain) *
                      oscillator(layer.shape, phase, held);
        }
    }
    lowpass(out, r.cutoff);
    float peak = 0.0f;
    for (std::size_t i = 0; i < n; ++i) {
        const double t = static_cast<double>(i) / kSampleRate;
        const double env = envelope(t, r.attack, r.length);
        out[i] *= static_cast<float>(env);
        peak = std::max(peak, std::abs(out[i]));
    }
    if (peak > 0.0f) {          // все звуки одной громкости
        for (float& s : out) {
            s *= 0.8f / peak;
        }
    }
    return out;
}

SoundBank::SoundBank() {
    for (std::size_t i = 0; i < kSoundCount; ++i) {
        waves_[i] = synthesize(kRecipes[i],
                               static_cast<unsigned>(i + 1));
    }
}

const Wave& SoundBank::operator[](SoundId id) const {
    return waves_[static_cast<std::size_t>(id)];
}

} // namespace doom
