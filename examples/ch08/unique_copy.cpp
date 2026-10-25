#include <memory>

struct Monster {
    int health = 20;
};

int main() {
    auto imp = std::make_unique<Monster>();
    auto twin = imp;                       // два владельца? нельзя
    auto heir = std::move(imp);            // передать — можно
    return heir->health + twin->health;
}
