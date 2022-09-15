# Knapsack-Solver

This is an assignment from CSE 5707 (Discrete Optimization) to write solvers for the Knapsack Problem. Included is a brute force, dynamic programming, and branch and bound solution.

## Brute Force

Mainly created for generating test cases, the brute force algorithm simply goes through all possible 2^n combinations of including or excluding an item from the knapsack. This brute force algorithm cycles through all 2^n combinations by using an integer bits as whether to include or exclude an item, and then incrementing that integer from 0 to 2^n. Where the n-th bit signals whether to include that item. The main benefit from this is that it runs in constant memory and does not need to store the entire power-set in memory. As a result, this can reasonably perform on a list of items with a size of about 30 within a minute on our laptops.

## Dynamic Programming

The idea of dynamic programming is to prevent doing the same work more than once. This is done by storing the result of *sub-problems* of the graph and then using the stored results again, rather than recomputing them. The main optimization made here was how to store these previous results. We ended uo encoding the sub-problem by the item it include or excludes and the remaining capacity into a 64 bit integer. That way it could be easily hashed in an unordered_map. This **significantly** reduces the number of nodes that the algorithm needs to explore and puts the brute force to shame by being able to find the solution to a 200 item-set in about 10 seconds.

## Branch and Bound

It can be shown that the (fractional) greedy solution to the knapsack problem serves as an upper bound to the optimal solution of the problem. This is where we greedily take the items with the highest value to weight ratios. And for the last item that does not fit into the pack, we take the value proportional to the amount of weight we can fit into the pack. This gives a non-discrete packing that optimally gives each weight the highest possible value. While this is not an optimal solution to the discrete case we want to solve, it serves as an upper bound for the possible solutions. What we can don with this bound is if we also have a lower bound (the best feasible solution thus far) and we have a sub-problem that has an upper bound lower than our feasible solution, then we know it is not worth exploring since it is not possible for it to be better than the solution we already have. Thus, we can cut a branch of exploration with a simple linear greedy algorithm.

The main problem of the Branch and Bound solution was how to traceback the nodes that give the optimal value. To do this, a significantly smaller instance of the nodes were stored in a list and had a link to the next meta-node, as they were named, and the item that the node opted to take. That way the size can be minimized without affecting the forward node exploration. This was also mainly to prevent the need for storing the list of selected items in every node in the exploration tree.

However, it was found that the branch and bound solution would generate significantly more nodes to explore than the DP solution... And would often take significantly longer.

## Usage

To build this, you need CMake and a compiler (gcc or clang for unix systems). 

```
./> git clone https://github.com/Same4254/Knapsack-Solver.git
./> cd Knapsack-Solver
./Knapsack-Solver/> mkdir build
./Knapsack-Solver/> cd build/
./Knapsack-Solver/build/> cmake -D CMAKE_BUILD_TYPE=RelWithDebInfo ..
./Knapsack-Solver/build/> make
./Knapsack-Solver/build/> ./Knapsack --instance <file-path>
```
