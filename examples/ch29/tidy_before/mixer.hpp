// mixer.hpp — микшер: складывает звучащие звуки в один поток
#pragma once

#include "synth.hpp"

#include <cstdint>
#include <mutex>
#include <span>
#include <vector>

namespace doom {

class Mixer {
public:
    explicit Mixer(const SoundBank& bank) : bank_(bank) {}

    // Из потока игры: «начни играть этот звук».
    void play(SoundId id, float volume);
    // Из звукового потока: «дай следующие out.size() сэмплов».
    void mix(std::span<std::int16_t> out);
    [[nodiscard]] std::size_t playing();

private:
    struct Voice {
        const Wave* wave = nullptr;
        std::size_t pos = 0;     // какой сэмпл следующий
        float volume = 1.0f;
    };
    static constexpr std::size_t kMaxVoices = 16;

    const SoundBank& bank_;
    std::mutex mutex_;           // охраняет voices_
    std::vector<Voice> voices_;
};

} // namespace doom
