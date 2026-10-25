#include <format>
#include <iostream>

int main() {
    int kills = 2;
    int total = 3;
    int truncated = kills * 100 / total;
    int rounded = (kills * 200 + total) / (2 * total);
    std::cout << std::format("truncated {}%, rounded {}%\n",
                             truncated, rounded);
}
