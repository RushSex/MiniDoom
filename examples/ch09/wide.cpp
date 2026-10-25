#include <iostream>
#include <string>

int main() {
    const std::string narrow = "DOOM";
    const std::wstring wide = L"DOOM";
    std::cout << "char:    " << sizeof(char) << " byte\n";
    std::cout << "wchar_t: " << sizeof(wchar_t) << " bytes\n";
    std::cout << "\"DOOM\":  " << narrow.size() * sizeof(char)
              << " bytes\n";
    std::cout << "L\"DOOM\": " << wide.size() * sizeof(wchar_t)
              << " bytes\n";
    std::cout << "long:    " << sizeof(long) << " bytes\n";
}
