#include <iostream>
#include <limits>

int main() {
    int score = std::numeric_limits<int>::max(); // самое большое int
    std::cout << "score: " << score << '\n';
    score = score + 1;                           // а теперь ещё очко
    std::cout << "score: " << score << '\n';
}
