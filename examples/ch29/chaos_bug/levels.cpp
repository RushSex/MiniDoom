// levels.cpp — разбор текста уровня, проверки и BFS до выхода
#include "levels.hpp"

#include "pathfind.hpp"

#include <algorithm>
#include <format>
#include <fstream>

namespace doom {

namespace {
constexpr std::string_view kCells = "123456789.PDXhaiz";

[[noreturn]] void fail(std::string_view source, int line,
                       std::string_view what) {
    throw LevelError(std::format("{}:{}: {}", source, line, what));
}

[[noreturn]] void fail(std::string_view source,
                       std::string_view what) {
    throw LevelError(std::format("{}: {}", source, what));
}

bool isWall(char c) { return c >= '1' && c <= '9'; }

// Проверки уровня целиком — когда все строки уже прочитаны.
void check(const Level& level, std::string_view source) {
    const std::size_t w = level.rows.front().size();
    const std::size_t h = level.rows.size();
    if (w < 3 || h < 3) {
        fail(source, "map is too small");
    }
    for (std::size_t y = 0; y < h; ++y) {   // по краю — только стены
        for (std::size_t x = 0; x < w; ++x) {
            const bool edge =
                x == 0 || y == 0 || x == w - 1 || y == h - 1;
            if (edge && !isWall(level.rows[y][x])) {  // дыра наружу
                const auto msg =
                    std::format("hole in outer wall at {},{}", x, y);
                fail(source, msg);
            }
        }
    }
    const Map map = level.map();
    const auto start = map.find('P');
    const auto exit = map.find('X');
    if (!start || !exit) {
        fail(source, "needs both P and X");
    }
    // Поиск в ширину от старта: дойдёт ли игрок до выхода?
    const std::vector<int> dist = distanceField(map, *start);
    const std::size_t i = cellIndex(exit->x, exit->y, map.width());
    if (dist[i] == kUnreached) {
        fail(source, std::format("exit at {},{} is unreachable",
                                 exit->x, exit->y));
    }
}
} // namespace

Map Level::map() const {
    // Map хочет string_view — делаем «окошки» в наши строки.
    const std::vector<std::string_view> views(rows.begin(),
                                              rows.end());
    return Map{views};
}

Level parseLevel(std::istream& in, std::string_view source) {
    Level level;
    std::string line;
    int number = 0;
    while (std::getline(in, line)) {
        ++number;
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();                // файл из Windows: \r\n
        }
        if (line.empty() || line.front() == '#') {
            continue;                       // пусто или комментарий
        }
        if (level.name.empty()) {           // первая строка — имя
            if (!line.starts_with("name ")) {
                fail(source, number, "expected 'name <title>'");
            }
            level.name = line.substr(5);
            continue;
        }
        const std::size_t width =
            level.rows.empty() ? line.size() : level.rows[0].size();
        if (line.size() != width) {
            fail(source, number,
                 std::format("row has {} cells, expected {}",
                             line.size(), width));
        }
        const auto bad = line.find_first_not_of(kCells);
        if (bad != std::string::npos) {
            fail(source, number,
                 std::format("unknown cell '{}' at column {}",
                             line[bad], bad));
        }
        level.rows.push_back(line);
    }
    if (level.rows.empty()) {
        fail(source, "no map");
    }
    check(level, source);
    return level;
}

Level loadLevel(const std::filesystem::path& path) {
    const std::string name = path.filename().string();
    std::ifstream file(path);
    if (!file) {
        fail(name, "cannot open");
    }
    return parseLevel(file, name);
}

std::vector<Level> loadLevels(const std::filesystem::path& dir) {
    namespace fs = std::filesystem;
    std::vector<fs::path> files;
    for (const auto& e : fs::directory_iterator(dir)) {
        if (e.path().extension() == ".lvl") {
            files.push_back(e.path());
        }
    }
    std::ranges::sort(files);   // порядок каталога не гарантирован
    std::vector<Level> levels;
    levels.reserve(files.size());
    for (const fs::path& f : files) {
        levels.push_back(loadLevel(f));
    }
    if (levels.empty()) {
        fail(dir.string(), "no *.lvl files here");
    }
    return levels;
}

} // namespace doom
