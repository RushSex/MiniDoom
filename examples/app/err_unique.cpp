#include <memory>

struct Imp {
    int hp = 60;
};

int main() {
    const auto a = std::make_unique<Imp>();
    const auto b = a;               // копия единоличного владельца?
    return b->hp;
}
