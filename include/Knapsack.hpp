#pragma once

#include <stdint.h>
#include <vector>

namespace Knapsack {
    struct Item {
        int64_t value;
        int64_t weight;

        Item(const int64_t value, const int64_t weight) : value(value), weight(weight) {

        }
    };

    struct KnapsackInstance {
        int64_t capacity;
        std::vector<Item> items;

        KnapsackInstance(const int64_t capacity, std::vector<Item> items) : capacity(capacity), items(items) {

        }
    };

    struct KnapsackResult {
        std::vector<Item> items;
    };

    KnapsackResult BruteForce(KnapsackInstance &instance);
};
