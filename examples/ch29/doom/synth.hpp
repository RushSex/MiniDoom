// synth.hpp — синтезатор: звуки из формул, без единого файла
#pragma once

#include "sound.hpp"

#include <array>
#include <vector>

namespace doom {

inline constexpr int kSampleRate = 22050;   // сэмплов в секунду

using Wave = std::vector<float>;            // сэмплы от −1 до 1

enum class Shape { Sine, Saw, Square, Noise };

// Один голос: форма волны и частота, которая плавно едет
// от from к to за время звука.
struct Layer {
    Shape shape = Shape::Sine;
    double from = 440.0;        // Гц в начале
    double to = 440.0;          // Гц в конце
    double gain = 0.0;          // 0 — слоя нет
};

struct Recipe {
    std::array<Layer, 2> layers;
    double attack = 0.01;       // с: нарастание громкости
    double length = 0.5;        // с: весь звук
    double cutoff = 0.0;        // Гц: фильтр; 0 — без фильтра
};

// Значение волны в точке phase ∈ [0, 1). held — для шума.
[[nodiscard]] float oscillator(Shape shape, double phase,
                               float held);
[[nodiscard]] Wave synthesize(const Recipe& r, unsigned seed);

// Все звуки игры, синтезированные один раз при запуске.
class SoundBank {
public:
    SoundBank();
    [[nodiscard]] const Wave& operator[](SoundId id) const;

private:
    std::array<Wave, kSoundCount> waves_;
};

} // namespace doom
