#include <iostream>

class Noisy {
public:
    explicit Noisy(char id) : id_(id) { std::cout << '+' << id_; }
    ~Noisy() { std::cout << '-' << id_; }

private:
    char id_;
};

int main() {
    Noisy a('a');
    {
        Noisy b('b');
        Noisy c('c');
    }
    Noisy d('d');
    std::cout << '|';
}
