#include <iostream>

class Health {
public:
    explicit Health(int max) : value_(max), max_(max) {}

    void damage(int amount) {
        value_ -= amount;
        if (value_ < 0) {
            value_ = 0;          // правило: не меньше нуля
        }
    }

    void heal(int amount) {
        value_ += amount;
        if (value_ > max_) {
            value_ = max_;       // правило: не больше максимума
        }
    }

    [[nodiscard]] int value() const { return value_; }
    [[nodiscard]] bool alive() const { return value_ > 0; }

private:
    int value_;
    int max_;
};

int main() {
    Health hero(100);
    std::cout << std::boolalpha;
    hero.damage(150);
    std::cout << "health: " << hero.value() << ", alive: "
              << hero.alive() << '\n';
    hero.heal(500);
    std::cout << "health: " << hero.value() << '\n';
}
