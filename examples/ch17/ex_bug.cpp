#include <iostream>

// «Случайное» число из координат — но на int.
int hash(int x, int y) {
    int h = x * 374761393 + y * 668265263;
    return h ^ (h >> 13);
}

int main() {
    std::cout << hash(1, 0) << '\n';
    std::cout << hash(7, 3) << '\n';
}
