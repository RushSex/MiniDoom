enum class Kind { Imp, Zombie, Baron };   // добавили третьего

int healthOf(Kind kind) {
    switch (kind) {
    case Kind::Imp:    return 20;
    case Kind::Zombie: return 10;
    }
    return 0;
}

int main() {
    return healthOf(Kind::Baron);
}
