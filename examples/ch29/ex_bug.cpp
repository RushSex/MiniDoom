#include <cassert>
#include <iostream>

int loaded = 0;

bool loadLevels() {
    ++loaded;                  // «загрузили» уровень
    return true;
}

int main() {
    assert(loadLevels());      // заодно проверим, что получилось
    std::cout << "levels loaded: " << loaded << '\n';
}
