#include <iostream>

int main() {
    const double damage = 12.7;
    int hp = 100;
    hp -= damage;                   // дробное в целое — молча?
    std::cout << hp << '\n';
}
