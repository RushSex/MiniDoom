#include <iostream>
#include <string>
#include <string_view>

struct Level {
    std::string_view name;     // экономим: не копируем строку
};

Level parseName(const std::string& line) {
    const std::string name = line.substr(5);   // без «name »
    Level level;
    level.name = name;
    return level;
}

int main() {
    const std::string line = "name THE LONG DARK CORRIDOR";
    const Level level = parseName(line);
    std::cout << level.name << '\n';
}
