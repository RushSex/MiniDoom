#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::vector<std::string> sounds{"pistol", "boom"};
    auto taken = std::exchange(sounds, {"door"});
    std::cout << taken.size() << ' ' << sounds.size() << ' '
              << sounds.front() << '\n';
}
