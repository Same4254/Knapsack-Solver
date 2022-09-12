#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <queue>

/**
 * Special cases to check for!
 *  - Capcity can hold all items (take everything without thinking much about it)
 *  - Item weight is greater than knapsack capacity (remove item before solving)
 *  - Item value of 0 (discard item before solving)
 *  - Item weight 0 (add item at the end of computation and don't make the solver figure that out)
 */
namespace Knapsack {
    struct Item {
        int64_t value;
        int64_t weight;

        Item(const int64_t value, const int64_t weight) : value(value), weight(weight) {

        }

        friend std::ostream& operator<<(std::ostream &stream, const Item &item) {
            stream << "Value: " << item.value << ", " << item.weight;
            return stream;
        }

        inline bool operator<(const Item &other) const {
            return value == other.value ? weight <= other.weight : value < other.value;
        }

        inline bool operator==(const Item &other) const {
            return value == other.value && weight == other.weight;
        }
    };

    struct KnapsackInstance {
        int64_t capacity;
        std::vector<Item> items;

        KnapsackInstance(const std::string &filePath);

        KnapsackInstance(const int64_t capacity, std::vector<Item> items) : capacity(capacity), items(items) {

        }

        void WriteToFile(const std::string &filePath);

        friend std::ostream& operator<<(std::ostream &stream, const KnapsackInstance &instance) {
            stream << "Num Elements: " << instance.items.size() << std::endl;
            for (auto &item : instance.items)
                stream << "\t" << item << std::endl;
            stream << "Capcity: " << instance.capacity << std::endl;
            return stream;
        }
    };

    struct KnapsackResult {
        std::vector<Item> items;
    
        bool operator==(const KnapsackResult &other) {
            std::vector<Item> sorted = items;
            std::vector<Item> otherSorted = other.items;

            std::sort(sorted.begin(), sorted.end());
            std::sort(otherSorted.begin(), otherSorted.end());
            
            return sorted == otherSorted;
        }
    };

    // ***** FILE I/O ***** 


    /**
     * Generates a randome problems instance with maximum constraints provided
     * The capacity of the knapsack is a random value between the weight of the heaviest item and the sum of all item weights.
     * This is because any value of capacity less than the heaviest item will begin to filter out items, which is not interesting to test. 
     * Any capacity greater than the sum of all weights is no different from an even higher capacity (solution: take everything).
     * These middle values for capacity are what will actually test the algorithm's decision
     */
    KnapsackInstance GenerateRandomInstance(const size_t minElements, const size_t maxElements, const int64_t maxItemValue, const int64_t maxItemWeight);

    // ***** Solvers *****

    KnapsackResult BruteForce(const KnapsackInstance &instance);
    KnapsackResult BruteForceFast(const KnapsackInstance &instance);

    struct BranchAndBoundNode {
        std::vector<Item> selectedItems;
        int64_t valueSum, weightSum;

        int64_t valueUpperBound;
        size_t index;

        BranchAndBoundNode(std::vector<Item> selectedItems, int64_t valueSum, int64_t weightSum, int64_t valueUpperBound, size_t index) : selectedItems(selectedItems), valueSum(valueSum), weightSum(weightSum), valueUpperBound(valueUpperBound), index(index) {

        }

        inline bool operator<(const BranchAndBoundNode &node2) const {
            return valueUpperBound < node2.valueUpperBound;
        }
    };

    KnapsackResult BranchAndBound(const KnapsackInstance &instance);
};
