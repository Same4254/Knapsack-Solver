#include "Knapsack.hpp"

using namespace Knapsack;

float CalculateBnBUpperBound(const KnapsackInstance &instance, const size_t startingIndex, int64_t remainingCapacity) {
    float upperBound = 0;
    for (size_t i = 0; i < instance.items.size(); i++) {
        const Item &item = instance.items[i];
        if (item.weight > remainingCapacity) {
            upperBound += item.value * ((float) remainingCapacity / (float) item.weight);
            return upperBound;
        } else {
            upperBound += item.value;
            remainingCapacity -= item.weight;
        }
    }

    return upperBound;
}

Knapsack::KnapsackResult Knapsack::BranchAndBound(const KnapsackInstance &instance) {
    KnapsackResult result;

    BranchAndBoundNode bestNode({}, 0, 0, CalculateBnBUpperBound(instance, 0, instance.capacity), 0);
    std::priority_queue<BranchAndBoundNode, std::vector<BranchAndBoundNode>, std::less<BranchAndBoundNode>> pq;
    pq.push(bestNode);

    while (!pq.empty()) {
        // Is there a better way to not reault in this copy?
        BranchAndBoundNode currentNode = pq.top();
        pq.pop();

        // include or exclude the item at this index
    }

    result.items = bestNode.selectedItems;

    return result;
}
