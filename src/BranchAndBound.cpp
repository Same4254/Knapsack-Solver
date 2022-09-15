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

    BranchAndBoundNode bestNode(0, 0, CalculateBnBUpperBound(instance, 0, instance.capacity), 0, 0);
    //std::priority_queue<BranchAndBoundNode, std::vector<BranchAndBoundNode>, std::less<BranchAndBoundNode>> pq;
    std::vector<BranchAndBoundNode> pq;
    //pq.push(bestNode);
    pq.push_back(bestNode);

    struct MetaNode {
        uint32_t nextIndex;
        uint32_t itemIndex;

        MetaNode(uint32_t nextIndex, uint32_t itemIndex) : nextIndex(nextIndex), itemIndex(itemIndex) {

        }
    };

    std::vector<MetaNode> tracebackInformation;
    // signifies when to stop tracing
    tracebackInformation.push_back(MetaNode(UINT32_MAX, UINT32_MAX));

    size_t count = 0;
    while (!pq.empty()) {
        count++;
        // Is there a better way to not reault in this copy?
        //BranchAndBoundNode currentNode = pq.top();
        //pq.pop();

        BranchAndBoundNode currentNode = pq[pq.size() - 1];
        pq.pop_back();

        if (currentNode.valueSum > bestNode.valueSum)
            bestNode = currentNode;

        // Performance note: if we have the list sorted by weight, we can stop if the rest of the weight of items is too big
        if (currentNode.itemIndex == instance.items.size() || currentNode.weightSum == instance.capacity) {
            continue;
        }

        // include current item
        if (instance.items[currentNode.itemIndex].weight + currentNode.weightSum <= instance.capacity) {
            int64_t newValueSum = currentNode.valueSum + instance.items[currentNode.itemIndex].value;
            int64_t newWeightSum = currentNode.weightSum + instance.items[currentNode.itemIndex].weight;
            
            if (newWeightSum <= instance.capacity) {
                // Stop evaluating if the upper bound is less than than our current solution
                float upperBound = (float) newValueSum + CalculateBnBUpperBound(instance, currentNode.itemIndex + 1, instance.capacity - newWeightSum);
                if (upperBound > bestNode.valueSum) {
                    pq.push_back(BranchAndBoundNode(newValueSum, newWeightSum, upperBound, currentNode.itemIndex + 1, tracebackInformation.size()));
                    tracebackInformation.push_back(MetaNode(currentNode.tracebackIndex, currentNode.itemIndex));
                }
            }
        }

        float upperBound = (float) currentNode.valueSum + CalculateBnBUpperBound(instance, currentNode.itemIndex + 1, instance.capacity - currentNode.weightSum);
        if (upperBound > bestNode.valueSum) {
            // exclude current item
            pq.push_back(BranchAndBoundNode(currentNode.valueSum, currentNode.weightSum, upperBound, currentNode.itemIndex + 1, tracebackInformation.size()));
            tracebackInformation.push_back(MetaNode(currentNode.tracebackIndex, UINT32_MAX));
        }
    }

    std::cout << "Branch and Bound Node Count: " << count << std::endl;

    MetaNode *node = &tracebackInformation[bestNode.tracebackIndex];
    while (!(node->itemIndex == UINT32_MAX && node->nextIndex == UINT32_MAX)) {
        if (node->itemIndex != UINT32_MAX)
            result.itemIndicies.insert(node->itemIndex);

        node = &tracebackInformation[node->nextIndex];
    }

    return result;
}
