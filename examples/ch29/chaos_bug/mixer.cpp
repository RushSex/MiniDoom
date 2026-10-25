// mixer.cpp — сложение голосов. Работает в двух потоках сразу.
#include "mixer.hpp"

#include <algorithm>

namespace doom {

void Mixer::play(SoundId id, float volume) {
    const std::scoped_lock lock(mutex_);
    if (voices_.size() == kMaxVoices) {
        voices_.erase(voices_.begin());   // самый старый — вон
    }
    voices_.push_back({&bank_[id], 0, volume});
}

void Mixer::mix(std::span<std::int16_t> out) {
    const std::scoped_lock lock(mutex_);
    for (std::int16_t& sample : out) {
        float sum = 0.0f;
        for (Voice& v : voices_) {
            if (v.pos < v.wave->size()) {
                sum += (*v.wave)[v.pos++] * v.volume;
            }
        }
        // Два громких звука в сумме вылезут за 1 — обрезаем.
        sum = std::clamp(sum, -1.0f, 1.0f);
        sample = static_cast<std::int16_t>(sum * 32767.0f);
    }
    std::erase_if(voices_, [](const Voice& v) {
        return v.pos >= v.wave->size();   // доиграл — свободен
    });
}

std::size_t Mixer::playing() {
    const std::scoped_lock lock(mutex_);
    return voices_.size();
}

} // namespace doom
