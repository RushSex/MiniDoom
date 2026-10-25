#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> scores{120, 450, 300};
    scores.push_back(990);
    int best = scores[0];
    for (int s : scores) {
        if (s > best) {
            best = s;
        }
    }
    std::cout << "best: " << best << '\n';
    std::cout << "via ranges: " << std::ranges::max(scores) << '\n';
}
