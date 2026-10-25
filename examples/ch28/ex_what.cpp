#include <iostream>
#include <stdexcept>

struct Guard {                 // говорит, когда умирает
    const char* name;
    ~Guard() { std::cout << '~' << name << ' '; }
};

void load(int n) {
    const Guard g{"file"};
    if (n > 1) {
        throw std::runtime_error("bad level");
    }
    std::cout << "ok ";
}

int main() {
    for (const int n : {1, 2}) {
        try {
            load(n);
        } catch (const std::exception& e) {
            std::cout << '[' << e.what() << "] ";
        }
    }
    std::cout << '\n';
}
