#include <string>

const std::string& levelName() {
    std::string name = "E1M1";
    return name;                      // ссылка на умирающую строку
}

int main() {
    return static_cast<int>(levelName().size());
}
