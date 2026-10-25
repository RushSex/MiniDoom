#include <iostream>

int main() {
    bool hasBlueKey = true;
    bool nearDoor = true;
    bool godMode = false;
    int health = 40;

    std::cout << std::boolalpha;
    // && — «и»: дверь откроется, только если есть ключ И ты рядом
    std::cout << "door opens:  " << (hasBlueKey && nearDoor) << '\n';
    // || — «или»: хватит здоровья ИЛИ включено бессмертие
    std::cout << "survives 50: " << (health > 50 || godMode) << '\n';
    // ! — «не»
    std::cout << "no key:      " << !hasBlueKey << '\n';
}
