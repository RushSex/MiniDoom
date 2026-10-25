#include <iostream>

int main() {
    char key = '1';
    switch (key) {
    case '1':
        std::cout << "Pistol\n";
    case '2':
        std::cout << "Shotgun\n";
        break;
    default:
        std::cout << "Nothing\n";
    }
}
