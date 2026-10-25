// audio.hpp — вывод звука на колонки в отдельном потоке
#pragma once

#include "mixer.hpp"

#include <stop_token>
#include <thread>

namespace doom {

class AudioOut {
public:
    explicit AudioOut(Mixer& mixer);

private:
    void run(std::stop_token stop);

    Mixer& mixer_;
    std::jthread thread_;   // последним: создаётся последним,
                            // уничтожается первым
};

} // namespace doom
