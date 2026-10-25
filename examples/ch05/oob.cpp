#include <iostream>
#include <vector>

int main() {
    std::vector<int> ammo{50, 8, 0};   // пули, дробь, ракеты
    std::cout << "rockets: " << ammo[2] << '\n';
    std::cout << "plasma:  " << ammo[3] << '\n';   // а такого нет!
}
