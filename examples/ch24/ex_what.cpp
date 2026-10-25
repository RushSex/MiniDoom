#include <iostream>
#include <queue>

int main() {
    std::queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    q.pop();
    q.push(4);
    std::cout << q.front() << ' ' << q.back() << ' ' << q.size()
              << '\n';
}
