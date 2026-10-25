#include <iostream>

[[nodiscard]] constexpr int clampHealth(int health) {
    return health < 0 ? 0 : (health > 100 ? 100 : health);
}

// Проверки идут при компиляции: одна ложна — программа не соберётся.
static_assert(clampHealth(130) == 100);
static_assert(clampHealth(-20) == 0);
static_assert(clampHealth(55) == 55);

int main() {
    std::cout << "all checks passed at compile time\n";
}
