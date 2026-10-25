class Health {
public:
    explicit Health(int max) : value_(max) {}
    [[nodiscard]] int value() const { return value_; }

private:
    int value_;
};

int main() {
    Health hero(100);
    hero.value_ = 9999;      // в обход правил
    return hero.value();
}
