#include <chrono>
#include <iostream>

#include "Knapsack.hpp"

int main() {
    using namespace Knapsack;

    //KnapsackInstance instance(50, {{60, 10}, {100, 20}, {120, 30}});
    KnapsackInstance instance = GenerateRandomInstance(30, 35, 20, 50);
    std::cout << instance.items.size() << std::endl;

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::seconds;
    using std::chrono::minutes;

    auto t1 = high_resolution_clock::now();
    //KnapsackResult res = BruteForce(instance);
    auto t2 = high_resolution_clock::now();

    std::cout << "Brute Force: " << duration_cast<minutes>(t2 - t1).count() << "m " << (duration_cast<seconds>(t2 - t1).count() % 60) << "s" << std::endl;

    t1 = high_resolution_clock::now();
    KnapsackResult res2 = BruteForceFast(instance);
    t2 = high_resolution_clock::now();

    std::cout << "Brute Force Fast: " << duration_cast<minutes>(t2 - t1).count() << "m " << (duration_cast<seconds>(t2 - t1).count() % 60) << "s" << std::endl;

    //std::cout << (res == res2) << std::endl;

    int64_t value = 0;
    int64_t weight = 0;

    for (const auto &item : res2.items) {
        value += item.value;
        weight += item.weight;
    }

    std::cout << "Optimal Value: " << value << std::endl;

    return 0;
}
