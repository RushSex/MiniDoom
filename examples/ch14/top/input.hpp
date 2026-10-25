// input.hpp — что сделал игрок; про Windows здесь ни слова
#pragma once

namespace doom {

struct Input {
    // Клавиши, которые сейчас зажаты.
    bool forward = false;       // W, стрелка вверх
    bool back = false;          // S, стрелка вниз
    bool strafeLeft = false;    // A — шаг влево, не поворачиваясь
    bool strafeRight = false;   // D
    bool turnLeft = false;      // стрелка влево
    bool turnRight = false;     // стрелка вправо
    bool run = false;           // Shift
    // События этого кадра.
    bool fire = false;
    int mouseDX = 0;            // сдвиг мыши по горизонтали
};

} // namespace doom
