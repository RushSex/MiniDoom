#include <format>
#include <iostream>
#include <string_view>

void printStat(std::string_view name, int value) {
    std::cout << std::format("{:<8}{:>6}\n", name, value);
}

void printStat(std::string_view name, double value, int prec = 1) {
    std::cout << std::format("{:<8}{:>6.{}f}\n", name, value, prec);
}

int main() {
    printStat("KILLS", 7);          // int    — первая версия
    printStat("SPEED", 3.4);        // double — вторая, digits = 1
    printStat("ACCUR", 0.8125, 3);  // double, три знака
}
