#include <chrono>
#include <iostream>

#include "Knapsack.hpp"
#include "DP.hpp"
#include <filesystem>

size_t countFailedFiles() {
    std::filesystem::path testCaseFolder("TestCases/Failed");

    size_t count = 0;
    for (auto& f : std::filesystem::directory_iterator(testCaseFolder))
        count++;

    return count;
}

void TimeAndTest(const KnapsackInstance &instance, bool writeFails) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::seconds;
    using std::chrono::minutes;

    //auto t1 = high_resolution_clock::now();
    //KnapsackResult res = BruteForce(instance);
    //auto t2 = high_resolution_clock::now();

    //int64_t optimal_value = 0;
    //for (auto i : res.itemIndicies)
    //    optimal_value += instance.items[i].value;

    //std::cout << "Brute Force: " << duration_cast<minutes>(t2 - t1).count() << "m " << (duration_cast<seconds>(t2 - t1).count() % 60) << "s" << std::endl;

    auto t1 = high_resolution_clock::now();
    KnapsackResult res2 = BruteForceFast(instance);
    auto t2 = high_resolution_clock::now();

    int64_t optimal_value = 0;
    for (auto i : res2.itemIndicies)
        optimal_value += instance.items[i].value;

    std::cout << "Brute Force Fast: " << duration_cast<minutes>(t2 - t1).count() << "m " << (duration_cast<seconds>(t2 - t1).count() % 60) << "s" << std::endl;

    t1 = high_resolution_clock::now();
    KnapsackResult res3 = DPKnapsack(instance);
    t2 = high_resolution_clock::now();

    std::cout << "Dynamic Programming:" << duration_cast<minutes>(t2 - t1).count() << "m " << (duration_cast<seconds>(t2 - t1).count() % 60) << "s" << std::endl;

    t1 = high_resolution_clock::now();
    KnapsackResult res4 = BranchAndBound(instance);
    t2 = high_resolution_clock::now();

    std::cout << "Branch and Bound: " << duration_cast<minutes>(t2 - t1).count() << "m " << (duration_cast<seconds>(t2 - t1).count() % 60) << "s" << std::endl;

    //std::cout << "Brute Force Fast: " << duration_cast<minutes>(t2 - t1).count() << "m " << (duration_cast<seconds>(t2 - t1).count() % 60) << "s" << std::endl;

    //std::cout << (res == res2) << std::endl;

    bool failed = false;
    //if (!(res == res2)) {
    //    failed = true;
    //    std::cout << "--Error!-- Brute Forces Do not agree!" << std::endl;
    //}

    if (!(res3 == res2)) {
        int64_t value = 0;
        for (auto i : res3.itemIndicies)
            value += instance.items[i].value;

        if (value != optimal_value) {
            failed = true;
            std::cout << "--Error!-- Dynamic Prgramming is not correct!" << std::endl;
        }
    }

    if (!(res4 == res2)) {
        int64_t value = 0;
        for (auto i : res4.itemIndicies)
            value += instance.items[i].value;

        if (value != optimal_value) {
            failed = true;
            std::cout << "--Error!-- Branch and Bound is not correct!" << std::endl;
        }
    }

    if (failed && writeFails) {
        static size_t failedFile = countFailedFiles();

        std::string fileName = "TestCases/Failed/Failed_" + std::to_string(failedFile) + ".txt";
        std::cout << "Writing to file! " << fileName << std::endl;
        instance.WriteToFile(fileName);
        failedFile++;
    }

    std::cout << std::endl;
}

void usage() {
    std::cout << "Usage:" << std::endl;
    std::cout << "\t--instance <file-path>" << std::endl;
}

int main(int argc, char *argv[]) {
    using namespace Knapsack;

    if (argc == 1) {
        //Testing stuff
        std::filesystem::path testCaseFolder("TestCases/Failed");
        if (!std::filesystem::exists(testCaseFolder)) {
            std::filesystem::create_directory(testCaseFolder);
        }

        for (auto& f : std::filesystem::directory_iterator(testCaseFolder)) {
            std::cout << "Testing: " << f.path().filename() << std::endl;
            TimeAndTest(KnapsackInstance(f.path().string()), false);
        }

        for(size_t i = 0; i < 100; i++) {
            KnapsackInstance instance = GenerateRandomInstance(15, 18, 20, 50);

            std::cout << "Item Count: " << instance.items.size() << std::endl;
            TimeAndTest(instance, true);
        }
    } else if(argc == 3) {
        std::string command(argv[1]);
        std::string value(argv[2]);
        if (command == "--instance") {
            KnapsackInstance instance(value);
            KnapsackResult result = BranchAndBound(instance);

            std::cout << "Items:" << std::endl;
            
            uint32_t value = 0;
            uint32_t weight = 0;
            for (auto i : result.itemIndicies) {
                value += instance.items[i].value;
                weight += instance.items[i].weight;

                std::cout << "Value: " << instance.items[i].value << ", Weight: " << instance.items[i].weight << std::endl;
            }

            for (auto &item : instance.zeroWeights) {
                value += item.value;
                weight += item.weight;

                std::cout << "Value: " << item.value << ", Weight: " << item.weight << std::endl;
            }

            std::cout << std::endl;
            std::cout << "Total Value: " << value << ", " << "Total Weight: " << weight << std::endl;
        } else {
            usage();
        }
    }
    
    return 0;
}
