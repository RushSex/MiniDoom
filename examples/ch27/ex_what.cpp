#include <iostream>
#include <string_view>

enum class Mode { Title, Playing, Paused };

// E — Enter, P — пауза (Esc).
Mode next(Mode m, char key) {
    switch (m) {
    case Mode::Title:
        return key == 'E' ? Mode::Playing : Mode::Title;
    case Mode::Playing:
        return key == 'P' ? Mode::Paused : Mode::Playing;
    case Mode::Paused:
        return key == 'P' || key == 'E' ? Mode::Playing
                                        : Mode::Paused;
    }
    return m;
}

int main() {
    Mode m = Mode::Title;
    for (const char key : std::string_view{"PEPPEP"}) {
        m = next(m, key);
        std::cout << static_cast<int>(m);
    }
    std::cout << '\n';
}
