#include <iostream>

int weaponDamage(char weapon) {
    if (weapon == 'p') {
        return 10;
    } else if (weapon == 's') {
        return 70;
    }
}

int main() {
    std::cout << weaponDamage('f') << '\n';
}
