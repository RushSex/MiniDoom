#include <format>
#include <iostream>

int main() {
    int levelTime = 83; // секунд
    int minutes = levelTime / 60;
    int seconds = levelTime % 60;
    std::cout << std::format("TIME {}:{:02}\n", minutes, seconds);
}
