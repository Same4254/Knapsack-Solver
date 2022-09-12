#pragma once

#include "Knapsack.hpp"
#include <unordered_map>

using namespace Knapsack;

// Since the table uses 64-bit identifiers for subproblems, 
// this assumes the capacity is at most 2^32 and the number of items is at most 2^32
struct DPKnapsack {
    const KnapsackInstance instance;
    std::unordered_map<uint64_t, KnapsackResult> table;

    DPKnapsack(const KnapsackInstance &instance) : instance(instance) {}
    KnapsackResult DPAlg();
    void DPSubProb(const int64_t remCapacity, const int64_t itemIndex);
};
