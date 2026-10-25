class Ammo {
public:
    explicit Ammo(int max) : max_(max), count_(max_ / 2) {}
    [[nodiscard]] int count() const { return count_; }

private:
    int count_;     // объявлен первым — и инициализируется первым!
    int max_;
};

int main() {
    Ammo pistol(200);
    return pistol.count() == 100 ? 0 : 1;
}
