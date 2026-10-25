#include <iostream>

struct Ammo {
    int bullets = 10;
    int shells = 0;
};

void addCopy(Ammo a) { a.bullets += 5; }
void addRef(Ammo& a) { a.shells += 4; }

int main() {
    Ammo box;
    Ammo other = box;     // копия целиком
    other.bullets = 99;
    addCopy(box);
    addRef(box);
    std::cout << box.bullets << ' ' << box.shells << ' '
              << other.bullets << '\n';
}
