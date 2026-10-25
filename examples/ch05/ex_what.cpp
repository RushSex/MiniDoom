#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<int> v{3, 1, 4};
    v.push_back(1);
    v[0] = v[1] + v[2];
    std::string s = "imp";
    s += s;
    std::cout << v.size() << ' ' << v[0] << ' ' << s << ' '
              << s.size() << '\n';
}
