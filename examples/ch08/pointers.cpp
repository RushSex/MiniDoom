#include <iostream>

int main() {
    int ammo = 50;
    int* p = &ammo;          // p хранит адрес ammo

    std::cout << std::boolalpha;
    std::cout << "p points to ammo: " << (p == &ammo) << '\n';
    std::cout << "*p = " << *p << '\n';   // «что лежит там»

    *p -= 10;                // меняем ammo через указатель
    std::cout << "ammo = " << ammo << '\n';

    p = nullptr;             // теперь никуда не указывает
    if (p == nullptr) {
        std::cout << "p points nowhere\n";
    }
}
