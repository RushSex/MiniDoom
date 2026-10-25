#include <iostream>
#include <memory>

class Monster {
public:
    explicit Monster(int health) : health_(health) {
        std::cout << "monster spawned\n";
    }
    ~Monster() { std::cout << "monster freed\n"; }
    [[nodiscard]] int health() const { return health_; }

private:
    int health_;
};

int main() {
    std::cout << "level start\n";
    {
        auto imp = std::make_unique<Monster>(60);   // владелец — imp
        std::cout << "imp health: " << imp->health() << '\n';
    }                       // imp умер — и сам освободил монстра
    std::cout << "level end\n";
}
