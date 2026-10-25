#include <iostream>
#include <vector>

int main() {
    std::vector<int> scores{120, 450, 300};
    int best = 0;
    for (std::size_t i = 0; i <= scores.size(); ++i) {
        if (scores[i] > best) {
            best = scores[i];
        }
    }
    std::cout << "best: " << best << '\n';
}
