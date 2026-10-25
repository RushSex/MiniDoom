// text.cpp — разворачиваем биты глифов в пиксели
#include "text.hpp"

#include "font.hpp"

namespace doom {

void drawText(Canvas& canvas, int x, int y, std::string_view text,
              std::uint32_t color, int scale) {
    for (char ch : text) {
        const Glyph glyph = glyphFor(ch);
        for (int row = 0; row < kGlyphH; ++row) {
            const auto bits = glyph[static_cast<std::size_t>(row)];
            for (int col = 0; col < kGlyphW; ++col) {
                if ((bits & (0x10 >> col)) == 0) {
                    continue;               // точка не закрашена
                }
                canvas.fillRect(x + col * scale, y + row * scale,
                                scale, scale, color);
            }
        }
        x += kGlyphAdvance * scale;         // к следующей букве
    }
}

void drawShadowText(Canvas& canvas, int x, int y,
                    std::string_view text, std::uint32_t color,
                    int scale) {
    const int off = scale > 1 ? scale / 2 : 1;
    drawText(canvas, x + off, y + off, text, rgb(0, 0, 0), scale);
    drawText(canvas, x, y, text, color, scale);
}

int textWidth(std::string_view text, int scale) {
    if (text.empty()) {
        return 0;
    }
    const auto n = static_cast<int>(text.size());
    return (n * kGlyphAdvance - 1) * scale;  // без хвостового зазора
}

} // namespace doom
