#include <iostream>

int main() {
    int a = 17;
    int b = 5;
    std::cout << a / b << ' ' << a % b << '\n';
    std::cout << a / 2.0 << ' ' << -a / b << '\n';
    double c = a / b;
    std::cout << c << '\n';
}
