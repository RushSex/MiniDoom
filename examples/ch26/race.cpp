#include <iostream>
#include <thread>

int main() {
    int counter = 0;
    // Два потока по миллиону раз прибавляют единицу к одной
    // переменной. Сколько получится?
    {
        std::jthread a([&counter] {
            for (int i = 0; i < 1'000'000; ++i) {
                ++counter;
            }
        });
        std::jthread b([&counter] {
            for (int i = 0; i < 1'000'000; ++i) {
                ++counter;
            }
        });
    }   // здесь jthread сами ждут завершения потоков
    std::cout << "counter = " << counter << '\n';
}
