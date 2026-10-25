#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> killed{"zombie", "imp"};
    killed.push_back("imp");        // ещё один в копилку
    killed.push_back("zombie");

    std::cout << "kills: " << killed.size() << '\n';
    for (std::size_t i = 0; i < killed.size(); ++i) {
        std::cout << "  #" << i << ": " << killed[i] << '\n';
    }
    std::cout << "first: " << killed.front()
              << ", last: " << killed.back() << '\n';
}
