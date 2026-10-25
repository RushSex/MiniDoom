#include <iostream>
#include <string_view>

enum class Kind { Imp, Zombie };

std::string_view nameOf(Kind kind) {
    switch (kind) {
    case Kind::Imp:    return "imp";
    case Kind::Zombie: return "zombie";
    }
    return "???";                // сюда не попадём
}

int healthOf(Kind kind) {
    return kind == Kind::Imp ? 20 : 10;
}

int main() {
    Kind k = Kind::Zombie;
    std::cout << nameOf(k) << " has " << healthOf(k) << " HP\n";
    k = Kind::Imp;
    std::cout << nameOf(k) << " has " << healthOf(k) << " HP\n";
}
