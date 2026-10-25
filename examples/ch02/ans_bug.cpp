#include <iostream>

int main() {
    int items = 4;
    int totalItems = 9;
    double percent = static_cast<double>(items) / totalItems * 100;
    std::cout << "ITEMS: " << percent << "%\n";
}
