#include <iostream>

class Gun {
public:
    int ammo() { return ammo_; }     // забыли const
private:
    int ammo_ = 20;
};

void show(const Gun& gun) {
    std::cout << gun.ammo() << '\n';
}

int main() {
    show(Gun{});
}
