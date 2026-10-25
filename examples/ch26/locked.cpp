#include <iostream>
#include <mutex>
#include <thread>

int main() {
    int counter = 0;
    std::mutex m;                       // замок на counter
    const auto work = [&] {
        for (int i = 0; i < 1'000'000; ++i) {
            const std::scoped_lock lock(m);   // взяли ключ…
            ++counter;
        }                                     // …и отдали
    };
    {
        std::jthread a(work);
        std::jthread b(work);
    }
    std::cout << "counter = " << counter << '\n';
}
