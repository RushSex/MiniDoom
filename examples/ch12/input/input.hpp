// input.hpp — что сделал игрок; про Windows здесь ни слова
#pragma once

namespace doom {

struct Input {
    // Клавиши, которые сейчас зажаты.
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool run = false;
    // Событие именно этого кадра: щелчок мышью.
    bool fire = false;
    // Сдвиг мыши за кадр, в «сырых» единицах мыши.
    int mouseDX = 0;
    int mouseDY = 0;
};

} // namespace doom
