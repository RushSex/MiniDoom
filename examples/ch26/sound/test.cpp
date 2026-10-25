// test.cpp — звуки: какие события рождает бой и что из них выходит
#include "game.hpp"
#include "level.hpp"
#include "mixer.hpp"
#include "synth.hpp"

#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {
// WAV — 44 байта заголовка и сырые 16-битные сэмплы.
void saveWav(const std::string& path,
             const std::vector<std::int16_t>& pcm) {
    std::ofstream f(path, std::ios::binary);
    const auto u32 = [&f](std::uint32_t v) {
        f.write(reinterpret_cast<const char*>(&v), 4);
    };
    const auto u16 = [&f](std::uint16_t v) {
        f.write(reinterpret_cast<const char*>(&v), 2);
    };
    const auto bytes = static_cast<std::uint32_t>(pcm.size() * 2);
    f << "RIFF";
    u32(36 + bytes);
    f << "WAVEfmt ";
    u32(16);
    u16(1);                                     // PCM
    u16(1);                                     // моно
    u32(doom::kSampleRate);
    u32(doom::kSampleRate * 2);                 // байт в секунду
    u16(2);
    u16(16);
    f << "data";
    u32(bytes);
    f.write(reinterpret_cast<const char*>(pcm.data()), bytes);
}

// Пропускаем звук через микшер, как в игре, и пишем в файл.
void render(const doom::SoundBank& bank, doom::SoundId id) {
    doom::Mixer mixer(bank);
    mixer.play(id, 1.0f);
    std::vector<std::int16_t> pcm(bank[id].size());
    mixer.mix(pcm);
    saveWav(std::format("{}.wav", doom::soundName(id)), pcm);
}
} // namespace

int main(int argc, char** argv) {
    const doom::SoundBank bank;
    if (argc > 1 && std::string_view(argv[1]) == "wav") {
        for (std::size_t i = 0; i < doom::kSoundCount; ++i) {
            const auto id = static_cast<doom::SoundId>(i);
            render(bank, id);
            const auto n = static_cast<double>(bank[id].size());
            const double sec = n / doom::kSampleRate;
            std::cout << std::format("{:>13}.wav  {:5.2f} s\n",
                                     doom::soundName(id), sec);
        }
        return 0;
    }
    // Бой из главы 25: стоим под огнём и выписываем все звуки.
    doom::Game game{doom::Map{doom::kLevel}};
    game.teleport(3.5, 11.5, 0.0);
    doom::Mixer mixer(bank);
    for (int frame = 1; frame <= 120; ++frame) {
        game.update({.fire = frame == 30}, 1.0 / 60.0);
        for (const doom::SoundEvent& e : game.takeSounds()) {
            mixer.play(e.id, e.volume);
            std::cout << std::format(
                "t {:4.2f}  {:<13} vol {:.2f}\n", frame / 60.0,
                doom::soundName(e.id), e.volume);
        }
        std::vector<std::int16_t> chunk(doom::kSampleRate / 60);
        mixer.mix(chunk);                       // кадр звука
    }
    std::cout << std::format("still playing: {}\n", mixer.playing());
}
