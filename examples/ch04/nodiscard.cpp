[[nodiscard]] int heal(int health, int amount) {
    const int result = health + amount;
    return result > 100 ? 100 : result;
}

int main() {
    int health = 40;
    heal(health, 25);     // вылечились? как бы не так
    return health;
}
