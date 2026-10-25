#include <iostream>
#include <string>
#include <string_view>

int main() {
    std::string_view title = std::string("E1M1") + ": Hangar";
    std::cout << title << '\n';
}
