#include <iostream>

int main() {
    int counter = 0;
    const auto byValue = [counter] { return counter + 1; };
    const auto byRef = [&counter] { return ++counter; };
    counter = 10;
    std::cout << byValue() << ' ' << byRef() << ' ' << counter
              << '\n';
}
