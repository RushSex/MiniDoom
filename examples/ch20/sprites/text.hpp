// text.hpp — вывод текста шрифтом 5×7 на холст
#pragma once

#include "canvas.hpp"

#include <cstdint>
#include <string_view>

namespace doom {

// Каждая точка буквы — квадрат scale × scale пикселей.
void drawText(Canvas& canvas, int x, int y, std::string_view text,
              std::uint32_t color, int scale = 1);

// Текст с тенью: сначала чёрный со сдвигом, потом цветной.
void drawShadowText(Canvas& canvas, int x, int y,
                    std::string_view text, std::uint32_t color,
                    int scale = 1);

[[nodiscard]] int textWidth(std::string_view text, int scale = 1);

} // namespace doom
