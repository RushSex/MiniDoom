#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

struct Thing {
    std::string name;
    double depth;
};

int main() {
    std::vector<Thing> things{{"medkit", 3.0}, {"clip", 7.5},
                              {"barrel", 1.2}, {"imp", 4.0}};
    std::ranges::sort(things, std::greater{}, &Thing::depth);
    for (const Thing& t : things) {
        std::cout << t.name << ' ';
    }
    std::cout << '\n';
}
