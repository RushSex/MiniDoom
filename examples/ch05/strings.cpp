#include <iostream>
#include <string>

int main() {
    std::string name = "Marine";
    std::string title = name + " of Mars";     // склейка строк

    std::cout << title << '\n';
    std::cout << "length: " << title.size() << '\n';
    std::cout << "first:  " << title[0] << '\n';
    std::cout << "has Mars at " << title.find("Mars") << '\n';
    std::cout << "piece:  " << title.substr(0, 6) << '\n';

    title += "!";                              // дописать в конец
    std::cout << title << '\n';
    std::cout << std::boolalpha << (name == "Marine") << '\n';
}
