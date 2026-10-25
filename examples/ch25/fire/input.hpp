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
    bool fire = false;          // левая кнопка мыши или Ctrl
    // События этого кадра.
    int weapon = 0;             // 1 или 2 — сменить оружие
    bool map = false;           // Tab: карта сверху / вид из глаз
    bool use = false;           // E или пробел: открыть дверь
    bool confirm = false;       // Enter: начать заново после смерти
    int mouseDX = 0;            // сдвиг мыши по горизонтали
};

} // namespace doom
