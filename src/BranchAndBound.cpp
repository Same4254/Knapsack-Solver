#include "Knapsack.hpp"

using namespace Knapsack;

float Knapsack::CalculateGreedyUpperBound(const KnapsackInstance &instance, const size_t startingIndex, int64_t remainingCapacity) {
    float upperBound = 0;
    for (size_t i = startingIndex; i < instance.items.size(); i++) {
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

    uint32_t lowerboundValue = 0;
    uint32_t lowerboundWeight = 0;
    for (size_t i = 0; i < instance.items.size(); i++) {
        if (lowerboundWeight + instance.items[i].weight > instance.capacity)
            break;
        lowerboundValue += instance.items[i].value;
        lowerboundWeight += instance.items[i].weight;
    }

    BranchAndBoundNode bestNode(0, 0, CalculateGreedyUpperBound(instance, 0, instance.capacity), 0, 0);
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

        if (currentNode.valueSum > lowerboundValue) {
            bestNode = currentNode;
            lowerboundValue = currentNode.valueSum;
        }

        if (currentNode.itemIndex == instance.items.size() || currentNode.weightSum == instance.capacity) {
            continue;
        }

        // include current item
        if (instance.items[currentNode.itemIndex].weight + currentNode.weightSum <= instance.capacity) {
            int64_t newValueSum = currentNode.valueSum + instance.items[currentNode.itemIndex].value;
            int64_t newWeightSum = currentNode.weightSum + instance.items[currentNode.itemIndex].weight;
            
            if (newWeightSum <= instance.capacity) {
                // Stop evaluating if the upper bound is less than than our current solution
                //float upperBound = (float) newValueSum + CalculateGreedyUpperBound(instance, currentNode.itemIndex + 1, instance.capacity - newWeightSum);
                //if (currentNode.valueUpperBound > lowerboundValue) {

                    pq.push_back(BranchAndBoundNode(newValueSum, newWeightSum, currentNode.valueUpperBound, currentNode.itemIndex + 1, tracebackInformation.size()));
                    tracebackInformation.push_back(MetaNode(currentNode.tracebackIndex, currentNode.itemIndex));
                //}
            }
        }

        float upperBound = (float) currentNode.valueSum + CalculateGreedyUpperBound(instance, currentNode.itemIndex + 1, instance.capacity - currentNode.weightSum);
        if (upperBound > lowerboundValue) {
            // exclude current item
            pq.push_back(BranchAndBoundNode(currentNode.valueSum, currentNode.weightSum, upperBound, currentNode.itemIndex + 1, tracebackInformation.size()));
            tracebackInformation.push_back(MetaNode(currentNode.tracebackIndex, UINT32_MAX));
        }
    }

    std::cout << "Branch and Bound Node Count: " << count << std::endl;

    if (bestNode.tracebackIndex == 0 && bestNode.itemIndex == 0) {
        uint32_t weightSum = 0;
        for (size_t i = 0; i < instance.items.size(); i++) {
            if (instance.items[i].weight + weightSum > instance.capacity)
                break;

            weightSum += instance.items[i].weight;
            result.itemIndicies.insert(i);
        }
    } else {
        MetaNode *node = &tracebackInformation[bestNode.tracebackIndex];
        while (!(node->itemIndex == UINT32_MAX && node->nextIndex == UINT32_MAX)) {
            if (node->itemIndex != UINT32_MAX)
                result.itemIndicies.insert(node->itemIndex);

            node = &tracebackInformation[node->nextIndex];
        }
    }

    return result;
}
