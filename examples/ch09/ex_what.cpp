#include <iostream>
#include <string>

int main() {
    const std::wstring title = L"MINI DOOM";
    std::cout << title.size() << ' '
              << title.size() * sizeof(wchar_t) << ' '
              << sizeof(L"MINI DOOM") << '\n';
}
