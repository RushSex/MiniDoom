#include <iostream>
#include <limits>

int main() {
    std::cout << "bool       " << sizeof(bool) << " byte\n";
    std::cout << "char       " << sizeof(char) << " byte\n";
    std::cout << "int        " << sizeof(int) << " bytes, max "
        << std::numeric_limits<int>::max() << '\n';
    std::cout << "long long  " << sizeof(long long) << " bytes, max "
        << std::numeric_limits<long long>::max() << '\n';
    std::cout << "float      " << sizeof(float) << " bytes, ~"
        << std::numeric_limits<float>::digits10 << " digits\n";
    std::cout << "double     " << sizeof(double) << " bytes, ~"
        << std::numeric_limits<double>::digits10 << " digits\n";
}
