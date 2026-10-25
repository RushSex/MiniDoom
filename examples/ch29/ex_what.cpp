#include <iostream>
#include <source_location>

void where(std::source_location at =
               std::source_location::current()) {
    std::cout << at.line() << ' ';
}

void helper() { where(); }

int main() {
    where();
    helper();
    helper();
    where();
    std::cout << '\n';
}
