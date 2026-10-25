// sound.hpp — какие звуки бывают. Игра знает только их имена.
#pragma once

#include <cstddef>
#include <string_view>

namespace doom {

enum class SoundId {
    Pistol, Shotgun, ImpSight, ZombieSight, MonsterPain,
    MonsterDeath, Fireball, Boom, Door, Pickup, PlayerHurt,
    PlayerDeath,
};
inline constexpr std::size_t kSoundCount = 12;

// «Где-то прозвучало»: что и насколько громко (0…1).
struct SoundEvent {
    SoundId id = SoundId::Pistol;
    float volume = 1.0f;
};

[[nodiscard]] std::string_view soundName(SoundId id);

} // namespace doom
