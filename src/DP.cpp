#include "Knapsack.hpp"
#include "DP.hpp"

using namespace Knapsack;

KnapsackResult DPKnapsack::DPAlg() {
    
    if (instance.items.size() == 0) { // In case there are no items
        return KnapsackResult();
    } else {
        DPSubProb(instance.capacity, 0);
        std::string fullProbId = std::to_string(instance.capacity) + "," + std::to_string(0);
        return table[fullProbId];
    }
}

void DPKnapsack::DPSubProb(const int64_t remCapacity, const int64_t itemIndex) {

    std::string subProbId = std::to_string(remCapacity) + "," + std::to_string(itemIndex); // Identifier of this subproblem

    int64_t untakenRemCapacity = remCapacity, takenRemCapacity = remCapacity - instance.items[itemIndex].weight;
    std::string untakenSubProbId = std::to_string(untakenRemCapacity) + "," + std::to_string(itemIndex + 1);
    std::string takenSubProbId = std::to_string(takenRemCapacity) + "," + std::to_string(itemIndex + 1);
    int64_t untakenProfit = 0, takenProfit = 0;
    
    // If we don't take item i
    if (itemIndex < (instance.items.size() - 1)) { // If this is not the last item
        if (table.count(untakenSubProbId) == 0) // If we don't yet have result for remainder
            DPSubProb(untakenRemCapacity, itemIndex + 1);
        // Find what would profit for remainder if we don't take item i
        for (const auto &item : table[untakenSubProbId].items)
            untakenProfit += item.value;
    }

    // If we can take item i (it fits)
    if (takenRemCapacity >= 0) {
        if (itemIndex < (instance.items.size() - 1)) { // If this is not the last item
            if (table.count(takenSubProbId) == 0) // If we don't yet have result for remainder
                DPSubProb(takenRemCapacity, itemIndex + 1);
            // Find what would be profit for remainder if we take item i
            for (const auto &item : table[takenSubProbId].items)
                takenProfit += item.value;
        }
        // Don't forget we take item i too
        takenProfit += instance.items[itemIndex].value;
    }

    // Pick best choice
    if (itemIndex == (instance.items.size() - 1)) { // If this is the last item
        table[subProbId] = KnapsackResult();
        if (untakenProfit < takenProfit) // Take item i
            table[subProbId].items.push_back(instance.items[itemIndex]);
    } else { // If this is not the last item
        if (untakenProfit >= takenProfit) { // Don't take item i
            table[subProbId] = table[untakenSubProbId];
        } else { // Take item i
            table[subProbId] = table[takenSubProbId];
            table[subProbId].items.push_back(instance.items[itemIndex]);
        }
    }

}



