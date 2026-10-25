#include <iostream>

int main() {
    char key = ' ';
    std::cout << "Choose weapon (1-3): ";
    std::cin >> key;

    switch (key) {
    case '1':
        std::cout << "Pistol. Reliable, but weak.\n";
        break;
    case '2':
        std::cout << "Shotgun. Now we're talking!\n";
        break;
    case '3':
        std::cout << "Fists. Brave. Stupid, but brave.\n";
        break;
    default:
        std::cout << "No such weapon.\n";
        break;
    }
}
