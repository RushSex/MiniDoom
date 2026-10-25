// levels.hpp — уровни в текстовых файлах: загрузка и проверка
#pragma once

#include "map.hpp"

#include <filesystem>
#include <istream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace doom {

struct Level {
    std::string name;                // как называется
    std::vector<std::string> rows;   // клетки, строка за строкой

    [[nodiscard]] Map map() const;
};

// Всё, что может пойти не так с файлом уровня.
class LevelError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// Разобрать уровень из любого потока: файла, строки, сети.
// source — имя для сообщений об ошибках.
[[nodiscard]] Level parseLevel(std::istream& in,
                               std::string_view source);
[[nodiscard]] Level loadLevel(const std::filesystem::path& path);
// Все *.lvl из папки, по алфавиту: e1m1, e1m2, …
[[nodiscard]] std::vector<Level> loadLevels(
    const std::filesystem::path& dir);

} // namespace doom
