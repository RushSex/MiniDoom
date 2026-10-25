#include <iostream>
#include <string>
#include <utility>

class Torch {
public:
    explicit Torch(std::string name) : name_(std::move(name)) {
        std::cout << "  [" << name_ << " lit]\n";
    }
    ~Torch() {                               // деструктор
        std::cout << "  [" << name_ << " out]\n";
    }

private:
    std::string name_;
};

int main() {
    std::cout << "enter the base\n";
    Torch hall("hall torch");
    {
        std::cout << "enter the cave\n";
        Torch cave("cave torch");
        Torch spare("spare torch");
        std::cout << "leave the cave\n";
    }                                        // тут гаснут оба факела
    std::cout << "leave the base\n";
}
