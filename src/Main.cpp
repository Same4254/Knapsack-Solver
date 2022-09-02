#include <iostream>

#include "Knapsack.hpp"

int main() {
    using namespace Knapsack;

    KnapsackInstance instance(50, {{60, 10}, {100, 20}, {120, 30}});
    KnapsackResult res = BruteForce(instance);

    int64_t value = 0;
    int64_t weight = 0;

    for (const auto &item : res.items) {
        value += item.value;
        weight += item.weight;
    }

    std::cout << "Optimal Value: " << value << std::endl;

    return 0;
}
