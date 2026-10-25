class Health {
public:
    explicit Health(int max) : value_(max) {}
    int value() { return value_; }       // забыли const
private:
    int value_;
};

int show(const Health& h) {
    return h.value();
}

int main() {
    Health hero(100);
    return show(hero);
}
