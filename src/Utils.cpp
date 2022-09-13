#include "Knapsack.hpp"

using namespace Knapsack;

// TODO: Could replace this with some file buffering and specific to the format (rather than using streams), for speed
KnapsackInstance::KnapsackInstance(const std::string &filePath) {
    std::ifstream file(filePath);
    int64_t numElements;

    std::string str;
    int64_t itemIndex;
    int64_t itemValue;
    int64_t itemWeight;

    file >> numElements;

    items.reserve(numElements);

    //std::cout << numElements << std::endl;
    for (size_t i = 0; i < numElements; i++) {
        file >> itemIndex >> itemValue >> itemWeight;
        //std::cout << itemIndex << ", " << itemValue << ", " << itemWeight << std::endl;

        items.push_back(Item(itemValue, itemWeight));
    }

    file >> capacity;

    //std::cout << capacity << std::endl;

    file.close();
}

void KnapsackInstance::WriteToFile(const std::string &filePath) const {
    std::ofstream file(filePath, std::ios::trunc);

    file << items.size() << std::endl;

    //TODO: Is this correct....?
    for (size_t i = 0; i < items.size(); i++)
        file << (i + 1) << " " << items[i].value << " " << items[i].weight << std::endl;

    file << capacity << std::endl;

    file.close();
}

KnapsackInstance Knapsack::GenerateRandomInstance(const size_t minElements, const size_t maxElements, const int64_t maxItemValue, const int64_t maxItemWeight) {
    std::random_device device;
    std::mt19937 rng(device());

    std::uniform_int_distribution<int64_t> elementRandomizer(minElements, maxElements);
    std::uniform_int_distribution<int64_t> valueRandomizer(1, maxItemValue);
    std::uniform_int_distribution<int64_t> weightRandomizer(1, maxItemValue);

    const size_t numElements = elementRandomizer(rng);

    std::vector<Item> items;
    items.reserve(numElements);

    int64_t weightSum = 0;
    int64_t maximumWeight = 0;
    for (size_t i = 0; i < numElements; i++) {
        int64_t tempWeight = weightRandomizer(rng);
        items.push_back(Item(valueRandomizer(rng), tempWeight));

        // Keep track of the highest weight and the sum of the weights
        weightSum += tempWeight;
        if (tempWeight > maximumWeight)
            maximumWeight = tempWeight;
    }

    std::uniform_int_distribution<int64_t> capacityRandomizer(maximumWeight, weightSum);

    return KnapsackInstance(capacityRandomizer(rng), items);
}
