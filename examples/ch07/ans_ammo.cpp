#include <algorithm>
#include <iostream>

class Ammo {
public:
    explicit Ammo(int max) : max_(max) {}

    // Подобрать патроны; лишнее сверх максимума пропадает.
    void add(int amount) {
        count_ = std::min(max_, count_ + amount);
    }

    // Потратить один патрон. false — если стрелять нечем.
    [[nodiscard]] bool spend() {
        if (count_ == 0) {
            return false;
        }
        --count_;
        return true;
    }

    [[nodiscard]] int count() const { return count_; }

private:
    int max_;
    int count_ = 0;
};

int main() {
    Ammo shells(50);
    shells.add(8);
    shells.add(100);
    int shots = 0;
    while (shells.spend()) {
        ++shots;
    }
    std::cout << "shots: " << shots << ", left: " << shells.count()
              << '\n';
}
