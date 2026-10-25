#include <iostream>

int main() {
    int a = 10;
    int b = 20;
    int* p = &a;
    int& r = b;
    *p += 5;
    p = &b;
    *p += r;
    r = 1;
    std::cout << a << ' ' << b << ' ' << *p << '\n';
}
