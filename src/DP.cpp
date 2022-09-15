#include "Knapsack.hpp"
#include "DP.hpp"

using namespace Knapsack;

// Since the table uses 64-bit identifiers for subproblems, this assumes the capacity <= 2^32 and number of items is <= 2^32
KnapsackResult DPKnapsack(const KnapsackInstance &instance) {
    if (instance.items.size() == 0) // In case there are no items
        return KnapsackResult();

    std::unordered_map<uint64_t, KnapsackResult> table;
    int64_t remCapacity = instance.capacity;
    int64_t itemIndex = 0;    
    uint64_t fullProbId = (remCapacity << 32) | itemIndex; // Identifier of full problem
    uint64_t subProbId = fullProbId; // Identifier of subproblem
    std::stack<uint64_t> subProbIdStack;
    subProbIdStack.push(subProbId);
    int64_t untakenRemCapacity, takenRemCapacity;
    uint64_t untakenSubProbId, takenSubProbId, untakenProfit, takenProfit;

    size_t count = 0;
    while (subProbIdStack.size() > 0) {
        count++;

        subProbId = subProbIdStack.top();
        remCapacity = subProbId >> 32;
        itemIndex = subProbId & (((uint64_t)1 << 32) - 1);
        untakenRemCapacity = remCapacity;
        takenRemCapacity = remCapacity - instance.items[itemIndex].weight;
        untakenSubProbId = (untakenRemCapacity << 32) | (itemIndex + 1);
        takenSubProbId = (takenRemCapacity << 32) | (itemIndex + 1);
        untakenProfit = 0;
        takenProfit = 0;
        
        // If we don't take item i
        if (itemIndex < (instance.items.size() - 1)) { // If this is not the last item
            if (table.count(untakenSubProbId) == 0) { // If we don't yet have result for remainder
                subProbIdStack.push(untakenSubProbId);
                continue;
            }
            // Find what would be profit for remainder if we don't take item i
            for (auto i : table[untakenSubProbId].itemIndicies) {
                const Item &item = instance.items[i];
                untakenProfit += item.value;
            }
        }

        // If we can take item i (it fits)
        if (takenRemCapacity >= 0) {
            if (itemIndex < (instance.items.size() - 1)) { // If this is not the last item
                if (table.count(takenSubProbId) == 0) { // If we don't yet have result for remainder
                    subProbIdStack.push(takenSubProbId);
                    continue;
                }
                // Find what would be profit for remainder if we take item i
                for (auto i : table[takenSubProbId].itemIndicies) {
                    const Item &item = instance.items[i];
                    takenProfit += item.value;
                }
            }
            // Don't forget we take item i too
            takenProfit += instance.items[itemIndex].value;
        }

        // Pick best choice
        if (itemIndex == (instance.items.size() - 1)) { // If this is the last item
            table[subProbId] = KnapsackResult();
            if (untakenProfit < takenProfit) // Take item i
                table[subProbId].itemIndicies.insert(itemIndex);
        } else { // If this is not the last item
            if (untakenProfit >= takenProfit) { // Don't take item i
                table[subProbId] = table[untakenSubProbId];
            } else { // Take item i
                table[subProbId] = table[takenSubProbId];
                table[subProbId].itemIndicies.insert(itemIndex);
            }
        }

        // Done here, go back up the stack
        subProbIdStack.pop();
    }

    std::cout << "Dynamic Programming Node Count: " << count << std::endl;

    return table[fullProbId];
}
