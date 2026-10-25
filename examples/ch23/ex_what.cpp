#include <iostream>

enum class State { Idle, Chase, Attack };

State next(State s, bool sees, bool close) {
    switch (s) {
    case State::Idle:
        return sees ? State::Chase : State::Idle;
    case State::Chase:
        return close ? State::Attack : State::Chase;
    case State::Attack:
        return State::Chase;
    }
    return s;
}

int main() {
    State s = State::Idle;
    const bool sees[] = {false, true, true, true, false};
    const bool close[] = {true, false, true, true, true};
    for (int i = 0; i < 5; ++i) {
        s = next(s, sees[i], close[i]);
        std::cout << static_cast<int>(s) << ' ';
    }
    std::cout << '\n';
}
