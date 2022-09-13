#include "Knapsack.hpp"

using namespace Knapsack;

template <typename T>
std::vector<std::vector<size_t>> PowerSet(const std::vector<T> &list) {
    std::vector<std::vector<size_t>> toRet;

    // Going through every number up to 2^n gives the 0 and 1 combinations of including and excluding the items
    // The nth bit of includer tells you if the nth element is included in the powerset
    // Only works up to 64 elements, but no way we are brute forcing this for that big of an itemset
    for (uint64_t includer = 0; includer < (1 << list.size()); includer++) {
        std::vector<size_t> temp;
        for (size_t i = 0; i < list.size(); i++) {
            // Test if the ith element is included
            if ((1 << i) & includer)
                temp.push_back(i);
        }

        toRet.push_back(temp);
    }

    return toRet;
}

KnapsackResult Knapsack::BruteForce(const KnapsackInstance &instance) {
    KnapsackResult result;
    std::vector<std::vector<size_t>> combinations = PowerSet(instance.items);

    int64_t bestValue = 0;
    int64_t bestIndex = -1;// -1 signifies no choice

    for (size_t i = 1; i < combinations.size(); i++) {
        int64_t value = 0;
        int64_t weight = 0;

        for (const auto index : combinations[i]) {
            const Item &item = instance.items[index];
            value += item.value;
            weight += item.weight;
        }

        if (weight <= instance.capacity && value > bestValue) {
            bestValue = value;
            bestIndex = i;
        }
    }

    if (bestIndex != -1)
        for (auto i : combinations[bestIndex])
            result.itemIndicies.insert(i);

    return result;
}

KnapsackResult Knapsack::BruteForceFast(const KnapsackInstance &instance) {
    KnapsackResult result;

    int64_t bestValue = 0;
    uint64_t bestChoice = 0;

    for (uint64_t includer = 0; includer < ((uint64_t)1 << (uint64_t)instance.items.size()); includer++) {
        int64_t value = 0;
        int64_t weight = 0;

        for (size_t i = 0; i < instance.items.size(); i++) {
            if ((1 << i) & includer) {
                value += instance.items[i].value;
                weight += instance.items[i].weight;
            }
        }

        if (weight <= instance.capacity && value > bestValue) {
            bestValue = value;
            bestChoice = includer;
        }
    }
    
    for (size_t i = 0; i < instance.items.size(); i++) {
        if ((1 << i) & bestChoice) {
            result.itemIndicies.insert(i);
        }
    }

    return result;
}
