#include <iostream>

int main() {
    int hp = 60;      // здоровье импа
    int damage = 15;  // урон за выстрел

    std::cout << "60 / 15 = " << hp / damage << '\n';
    std::cout << "60 / 7  = " << hp / 7 << '\n';   // целое деление!
    std::cout << "60 % 7  = " << hp % 7 << '\n';   // остаток
    std::cout << "60.0 / 7 = " << 60.0 / 7 << '\n';
    std::cout << "7 / 2 * 2 = " << 7 / 2 * 2 << '\n';
}
