#pragma once

#include "Knapsack.hpp"
#include <map>

using namespace Knapsack;

struct DPKnapsack {
    const KnapsackInstance instance;
    std::map<std::string, KnapsackResult> table;

    DPKnapsack(const KnapsackInstance &instance) : instance(instance) {}
    KnapsackResult DPAlg();
    void DPSubProb(const int64_t remCapacity, const int64_t itemIndex);
};
