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

KnapsackResult Knapsack::BranchAndBound(const KnapsackInstance &instance) {
    KnapsackResult result;

    BranchAndBoundNode bestNode({}, 0, 0, CalculateBnBUpperBound(instance, 0, instance.capacity), 0);
    std::priority_queue<BranchAndBoundNode, std::vector<BranchAndBoundNode>, std::less<BranchAndBoundNode>> pq;
    pq.push(bestNode);

    while (!pq.empty()) {
        // Is there a better way to not reault in this copy?
        BranchAndBoundNode currentNode = pq.top();
        pq.pop();

        // terminating condition. Assuming there is no weight of 0
        if (currentNode.valueSum > bestNode.valueSum)
            bestNode = currentNode;

        // Performance note: if we have the list sorted by weight, we can stop if the rest of the weight of items is too big
        if (currentNode.index == instance.items.size() || currentNode.weightSum == instance.capacity) {
            continue;
        }

        // include current item
        if (instance.items[currentNode.index].weight + currentNode.weightSum <= instance.capacity) {
            int64_t newValueSum = currentNode.valueSum + instance.items[currentNode.index].value;
            int64_t newWeightSum = currentNode.weightSum + instance.items[currentNode.index].weight;

            BranchAndBoundNode node(currentNode.selectedIndecies, newValueSum, newWeightSum, newValueSum + CalculateBnBUpperBound(instance, currentNode.index + 1, instance.capacity - newWeightSum), currentNode.index + 1);
            node.selectedIndecies.push_back(currentNode.index);

            pq.push(BranchAndBoundNode(node));
        }

        // exclude current item
        pq.push(BranchAndBoundNode(currentNode.selectedIndecies, currentNode.valueSum, currentNode.weightSum, currentNode.valueSum + CalculateBnBUpperBound(instance, currentNode.index + 1, instance.capacity - currentNode.weightSum), currentNode.index + 1));
    }

    for (auto i : bestNode.selectedIndecies)
        result.itemIndicies.insert(i);

    return result;
}
